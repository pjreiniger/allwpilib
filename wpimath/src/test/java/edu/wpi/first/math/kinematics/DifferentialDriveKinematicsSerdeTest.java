// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Kinematics.ProtobufDifferentialDriveKinematics;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class DifferentialDriveKinematicsSerdeTest {
  private static final DifferentialDriveKinematics DATA = new DifferentialDriveKinematics(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(DifferentialDriveKinematics.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    DifferentialDriveKinematics.struct.pack(buffer, DATA);

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

    DifferentialDriveKinematics data = DifferentialDriveKinematics.struct.unpack(buffer);
    assertEquals(DATA.getTrackWidthMeters(), data.getTrackWidthMeters());
  }

  @Test
  void testProtoPack() {
    ProtobufDifferentialDriveKinematics proto = DifferentialDriveKinematics.proto.createMessage();
    DifferentialDriveKinematics.proto.pack(proto, DATA);

    assertEquals(DATA.getTrackWidthMeters(), proto.getTrackWidthMeters());
  }

  @Test
  void testProtoUnpack() {
    ProtobufDifferentialDriveKinematics proto = DifferentialDriveKinematics.proto.createMessage();
    proto.setTrackWidthMeters(DATA.getTrackWidthMeters());

    DifferentialDriveKinematics data = DifferentialDriveKinematics.proto.unpack(proto);
    assertEquals(DATA.getTrackWidthMeters(), data.getTrackWidthMeters());
  }
}
