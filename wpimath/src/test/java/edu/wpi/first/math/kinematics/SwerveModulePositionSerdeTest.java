// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Kinematics.ProtobufSwerveModulePosition;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class SwerveModulePositionSerdeTest {
  private static final SwerveModulePosition DATA = new SwerveModulePosition(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(SwerveModulePosition.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    SwerveModulePosition.struct.pack(buffer, DATA);

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

    SwerveModulePosition data = SwerveModulePosition.struct.unpack(buffer);
    assertEquals(DATA.getDistanceMeters(), data.getDistanceMeters());
    assertEquals(DATA.getAngle(), data.getAngle());
  }

  @Test
  void testProtoPack() {
    ProtobufSwerveModulePosition proto = SwerveModulePosition.proto.createMessage();
    SwerveModulePosition.proto.pack(proto, DATA);

    assertEquals(DATA.getDistanceMeters(), proto.getDistanceMeters());
    assertEquals(DATA.getAngle(), Rotation2d.proto.unpack(proto.getAngle()));
  }

  @Test
  void testProtoUnpack() {
    ProtobufSwerveModulePosition proto = SwerveModulePosition.proto.createMessage();
    proto.setDistanceMeters(DATA.getDistanceMeters());
    Rotation2d.proto.pack(proto.getMutableAngle(), DATA.getAngle());

    SwerveModulePosition data = SwerveModulePosition.proto.unpack(proto);
    assertEquals(DATA.getDistanceMeters(), data.getDistanceMeters());
    assertEquals(DATA.getAngle(), data.getAngle());
  }
}
