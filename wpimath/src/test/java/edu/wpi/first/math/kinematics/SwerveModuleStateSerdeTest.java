// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto..ProtobufSwerveModuleState;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class SwerveModuleStateSerdeTest {
  private static final SwerveModuleState DATA = new SwerveModuleState(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(SwerveModuleState.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    SwerveModuleState.struct.pack(buffer, DATA);

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

    SwerveModuleState data = SwerveModuleState.struct.unpack(buffer);
    assertEquals(DATA.speedMetersPerSecond, data.speedMetersPerSecond);
    assertEquals(DATA.getAngle(), data.getAngle());
  }

  @Test
  void testProtoPack() {
    ProtobufSwerveModuleState proto = SwerveModuleState.proto.createMessage();
    SwerveModuleState.proto.pack(proto, DATA);

    assertEquals(DATA.speedMetersPerSecond, proto.getSpeedMps());
    assertEquals(DATA.getAngle(), proto.getAngle());
  }

  @Test
  void testProtoUnpack() {
    ProtobufSwerveModuleState proto = SwerveModuleState.proto.createMessage();
    proto.setSpeedMps(DATA.speedMetersPerSecond);
    proto.getMutableAngle(DATA.getAngle());

    SwerveModuleState data = SwerveModuleState.proto.unpack(proto);
    assertEquals(DATA.speedMetersPerSecond, data.speedMetersPerSecond);
    assertEquals(DATA.getAngle(), data.getAngle());
  }
}
