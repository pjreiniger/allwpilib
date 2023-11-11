// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Kinematics.ProtobufMecanumDriveMotorVoltages;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class MecanumDriveMotorVoltagesSerdeTest {
  private static final MecanumDriveMotorVoltages DATA = new MecanumDriveMotorVoltages(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(MecanumDriveMotorVoltages.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    MecanumDriveMotorVoltages.struct.pack(buffer, DATA);

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

    MecanumDriveMotorVoltages data = MecanumDriveMotorVoltages.struct.unpack(buffer);
    assertEquals(DATA.getFrontLeftVolts(), data.getFrontLeftVolts());
    assertEquals(DATA.getFrontRightVolts(), data.getFrontRightVolts());
    assertEquals(DATA.getRearLeftVolts(), data.getRearLeftVolts());
    assertEquals(DATA.getRearRightVolts(), data.getRearRightVolts());
  }

  @Test
  void testProtoPack() {
    ProtobufMecanumDriveMotorVoltages proto = MecanumDriveMotorVoltages.proto.createMessage();
    MecanumDriveMotorVoltages.proto.pack(proto, DATA);

    assertEquals(DATA.getFrontLeftVolts(), proto.getFrontLeftVolts());
    assertEquals(DATA.getFrontRightVolts(), proto.getFrontRightVolts());
    assertEquals(DATA.getRearLeftVolts(), proto.getRearLeftVolts());
    assertEquals(DATA.getRearRightVolts(), proto.getRearRightVolts());
  }

  @Test
  void testProtoUnpack() {
    ProtobufMecanumDriveMotorVoltages proto = MecanumDriveMotorVoltages.proto.createMessage();
    proto.setFrontLeftVolts(DATA.getFrontLeftVolts());
    proto.setFrontRightVolts(DATA.getFrontRightVolts());
    proto.setRearLeftVolts(DATA.getRearLeftVolts());
    proto.setRearRightVolts(DATA.getRearRightVolts());

    MecanumDriveMotorVoltages data = MecanumDriveMotorVoltages.proto.unpack(proto);
    assertEquals(DATA.getFrontLeftVolts(), data.getFrontLeftVolts());
    assertEquals(DATA.getFrontRightVolts(), data.getFrontRightVolts());
    assertEquals(DATA.getRearLeftVolts(), data.getRearLeftVolts());
    assertEquals(DATA.getRearRightVolts(), data.getRearRightVolts());
  }
}
