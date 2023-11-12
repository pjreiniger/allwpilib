// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.trajectory;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto..ProtobufTrajectory;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class TrajectorySerdeTest {
  private static final Trajectory DATA = new Trajectory(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Trajectory.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    Trajectory.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    String newContent = new String(buffer.array());
    System.out.println(Arrays.toString(actual));
    System.out.println(newContent);
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);
    buffer.order(ByteOrder.LITTLE_ENDIAN);

    Trajectory data = Trajectory.struct.unpack(buffer);
    assertEquals(DATA.totalTime, data.totalTime);
    assertEquals(DATA.getStates(), data.getStates());
  }

  @Test
  void testProtoPack() {
    ProtobufTrajectory proto = Trajectory.proto.createMessage();
    Trajectory.proto.pack(proto, DATA);

    assertEquals(DATA.totalTime, proto.getTotalTime());
    assertEquals(DATA.getStates(), proto.getStates());
  }

  @Test
  void testProtoUnpack() {
    ProtobufTrajectory proto = Trajectory.proto.createMessage();
    proto.setTotalTime(DATA.totalTime);
    proto.getMutableStates(DATA.getStates());

    Trajectory data = Trajectory.proto.unpack(proto);
    assertEquals(DATA.totalTime, data.totalTime);
    assertEquals(DATA.getStates(), data.getStates());
  }
}
