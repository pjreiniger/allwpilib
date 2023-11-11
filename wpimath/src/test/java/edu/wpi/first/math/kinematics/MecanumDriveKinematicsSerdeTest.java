// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Kinematics.ProtobufMecanumDriveKinematics;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class MecanumDriveKinematicsSerdeTest {
  private static final MecanumDriveKinematics DATA = new MecanumDriveKinematics(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(MecanumDriveKinematics.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    MecanumDriveKinematics.struct.pack(buffer, DATA);

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

    MecanumDriveKinematics data = MecanumDriveKinematics.struct.unpack(buffer);
    assertEquals(DATA.getFrontLeft(), data.getFrontLeft());
    assertEquals(DATA.getFrontRight(), data.getFrontRight());
    assertEquals(DATA.getRearLeft(), data.getRearLeft());
    assertEquals(DATA.getRearRight(), data.getRearRight());
  }

  @Test
  void testProtoPack() {
    ProtobufMecanumDriveKinematics proto = MecanumDriveKinematics.proto.createMessage();
    MecanumDriveKinematics.proto.pack(proto, DATA);

    assertEquals(DATA.getFrontLeft(), Translation2d.proto.unpack(proto.getFrontLeft()));
    assertEquals(DATA.getFrontRight(), Translation2d.proto.unpack(proto.getFrontRight()));
    assertEquals(DATA.getRearLeft(), Translation2d.proto.unpack(proto.getRearLeft()));
    assertEquals(DATA.getRearRight(), Translation2d.proto.unpack(proto.getRearRight()));
  }

  @Test
  void testProtoUnpack() {
    ProtobufMecanumDriveKinematics proto = MecanumDriveKinematics.proto.createMessage();
    Translation2d.proto.pack(proto.getMutableFrontLeft(), DATA.getFrontLeft());
    Translation2d.proto.pack(proto.getMutableFrontRight(), DATA.getFrontRight());
    Translation2d.proto.pack(proto.getMutableRearLeft(), DATA.getRearLeft());
    Translation2d.proto.pack(proto.getMutableRearRight(), DATA.getRearRight());

    MecanumDriveKinematics data = MecanumDriveKinematics.proto.unpack(proto);
    assertEquals(DATA.getFrontLeft(), data.getFrontLeft());
    assertEquals(DATA.getFrontRight(), data.getFrontRight());
    assertEquals(DATA.getRearLeft(), data.getRearLeft());
    assertEquals(DATA.getRearRight(), data.getRearRight());
  }
}
