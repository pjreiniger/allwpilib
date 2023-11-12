// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto..ProtobufMecanumDriveWheelSpeeds;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class MecanumDriveWheelSpeedsSerdeTest {
  private static final MecanumDriveWheelSpeeds DATA = new MecanumDriveWheelSpeeds(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(MecanumDriveWheelSpeeds.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    MecanumDriveWheelSpeeds.struct.pack(buffer, DATA);

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

    MecanumDriveWheelSpeeds data = MecanumDriveWheelSpeeds.struct.unpack(buffer);
    assertEquals(DATA.frontLeftMetersPerSecond, data.frontLeftMetersPerSecond);
    assertEquals(DATA.frontRightMetersPerSecond, data.frontRightMetersPerSecond);
    assertEquals(DATA.rearLeftMetersPerSecond, data.rearLeftMetersPerSecond);
    assertEquals(DATA.rearRightMetersPerSecond, data.rearRightMetersPerSecond);
  }

  @Test
  void testProtoPack() {
    ProtobufMecanumDriveWheelSpeeds proto = MecanumDriveWheelSpeeds.proto.createMessage();
    MecanumDriveWheelSpeeds.proto.pack(proto, DATA);

    assertEquals(DATA.frontLeftMetersPerSecond, proto.getFrontLeftMps());
    assertEquals(DATA.frontRightMetersPerSecond, proto.getFrontRightMps());
    assertEquals(DATA.rearLeftMetersPerSecond, proto.getRearLeftMps());
    assertEquals(DATA.rearRightMetersPerSecond, proto.getRearRightMps());
  }

  @Test
  void testProtoUnpack() {
    ProtobufMecanumDriveWheelSpeeds proto = MecanumDriveWheelSpeeds.proto.createMessage();
    proto.setFrontLeftMps(DATA.frontLeftMetersPerSecond);
    proto.setFrontRightMps(DATA.frontRightMetersPerSecond);
    proto.setRearLeftMps(DATA.rearLeftMetersPerSecond);
    proto.setRearRightMps(DATA.rearRightMetersPerSecond);

    MecanumDriveWheelSpeeds data = MecanumDriveWheelSpeeds.proto.unpack(proto);
    assertEquals(DATA.frontLeftMetersPerSecond, data.frontLeftMetersPerSecond);
    assertEquals(DATA.frontRightMetersPerSecond, data.frontRightMetersPerSecond);
    assertEquals(DATA.rearLeftMetersPerSecond, data.rearLeftMetersPerSecond);
    assertEquals(DATA.rearRightMetersPerSecond, data.rearRightMetersPerSecond);
  }
}
