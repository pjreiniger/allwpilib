// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics.serde;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.kinematics.MecanumDriveMotorVoltages;
import edu.wpi.first.math.proto.Kinematics.ProtobufMecanumDriveMotorVoltages;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;

public class MecanumDriveMotorVoltagesSerdeTest {
  private static final MecanumDriveMotorVoltages DATA =
      new MecanumDriveMotorVoltages(1.91, 2.29, 3.504, 1.74);
  private static final byte[] STRUCT_BUFFER =
      new byte[] {
        -113, -62, -11, 40, 92, -113, -2, 63, 82, -72, 30, -123, -21, 81, 2, 64, -43, 120, -23, 38,
        49, 8, 12, 64, -41, -93, 112, 61, 10, -41, -5, 63
      };

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(MecanumDriveMotorVoltages.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    MecanumDriveMotorVoltages.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);
    buffer.order(ByteOrder.LITTLE_ENDIAN);

    MecanumDriveMotorVoltages data = MecanumDriveMotorVoltages.struct.unpack(buffer);
    assertEquals(DATA.frontLeftVoltage, data.frontLeftVoltage);
    assertEquals(DATA.frontRightVoltage, data.frontRightVoltage);
    assertEquals(DATA.rearLeftVoltage, data.rearLeftVoltage);
    assertEquals(DATA.rearRightVoltage, data.rearRightVoltage);
  }

  @Test
  void testProtoPack() {
    ProtobufMecanumDriveMotorVoltages proto = MecanumDriveMotorVoltages.proto.createMessage();
    MecanumDriveMotorVoltages.proto.pack(proto, DATA);

    assertEquals(DATA.frontLeftVoltage, proto.getFrontLeftVolts());
    assertEquals(DATA.frontRightVoltage, proto.getFrontRightVolts());
    assertEquals(DATA.rearLeftVoltage, proto.getRearLeftVolts());
    assertEquals(DATA.rearRightVoltage, proto.getRearRightVolts());
  }

  @Test
  void testProtoUnpack() {
    ProtobufMecanumDriveMotorVoltages proto = MecanumDriveMotorVoltages.proto.createMessage();
    proto.setFrontLeftVolts(DATA.frontLeftVoltage);
    proto.setFrontRightVolts(DATA.frontRightVoltage);
    proto.setRearLeftVolts(DATA.rearLeftVoltage);
    proto.setRearRightVolts(DATA.rearRightVoltage);

    MecanumDriveMotorVoltages data = MecanumDriveMotorVoltages.proto.unpack(proto);
    assertEquals(DATA.frontLeftVoltage, data.frontLeftVoltage);
    assertEquals(DATA.frontRightVoltage, data.frontRightVoltage);
    assertEquals(DATA.rearLeftVoltage, data.rearLeftVoltage);
    assertEquals(DATA.rearRightVoltage, data.rearRightVoltage);
  }
}
