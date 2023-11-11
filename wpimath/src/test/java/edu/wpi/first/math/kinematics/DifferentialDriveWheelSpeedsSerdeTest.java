// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Kinematics.ProtobufDifferentialDriveWheelSpeeds;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class DifferentialDriveWheelSpeedsSerdeTest {
  private static final DifferentialDriveWheelSpeeds DATA = new DifferentialDriveWheelSpeeds(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(DifferentialDriveWheelSpeeds.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    DifferentialDriveWheelSpeeds.struct.pack(buffer, DATA);

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

    DifferentialDriveWheelSpeeds data = DifferentialDriveWheelSpeeds.struct.unpack(buffer);
    assertEquals(DATA.getLeftMps(), data.getLeftMps());
    assertEquals(DATA.getRightMps(), data.getRightMps());
  }

  @Test
  void testProtoPack() {
    ProtobufDifferentialDriveWheelSpeeds proto = DifferentialDriveWheelSpeeds.proto.createMessage();
    DifferentialDriveWheelSpeeds.proto.pack(proto, DATA);

    assertEquals(DATA.getLeftMps(), proto.getLeftMps());
    assertEquals(DATA.getRightMps(), proto.getRightMps());
  }

  @Test
  void testProtoUnpack() {
    ProtobufDifferentialDriveWheelSpeeds proto = DifferentialDriveWheelSpeeds.proto.createMessage();
    proto.setLeftMps(DATA.getLeftMps());
    proto.setRightMps(DATA.getRightMps());

    DifferentialDriveWheelSpeeds data = DifferentialDriveWheelSpeeds.proto.unpack(proto);
    assertEquals(DATA.getLeftMps(), data.getLeftMps());
    assertEquals(DATA.getRightMps(), data.getRightMps());
  }
}
