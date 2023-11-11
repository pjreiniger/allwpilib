// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Controller.ProtobufDifferentialDriveWheelVoltages;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class DifferentialDriveWheelVoltagesSerdeTest {
  private static final DifferentialDriveWheelVoltages DATA = new DifferentialDriveWheelVoltages(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(DifferentialDriveWheelVoltages.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    DifferentialDriveWheelVoltages.struct.pack(buffer, DATA);

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

    DifferentialDriveWheelVoltages data = DifferentialDriveWheelVoltages.struct.unpack(buffer);
    assertEquals(DATA.getLeft(), data.getLeft());
    assertEquals(DATA.getRight(), data.getRight());
  }

  @Test
  void testProtoPack() {
    ProtobufDifferentialDriveWheelVoltages proto = DifferentialDriveWheelVoltages.proto.createMessage();
    DifferentialDriveWheelVoltages.proto.pack(proto, DATA);

    assertEquals(DATA.getLeft(), proto.getLeft());
    assertEquals(DATA.getRight(), proto.getRight());
  }

  @Test
  void testProtoUnpack() {
    ProtobufDifferentialDriveWheelVoltages proto = DifferentialDriveWheelVoltages.proto.createMessage();
    proto.setLeft(DATA.getLeft());
    proto.setRight(DATA.getRight());

    DifferentialDriveWheelVoltages data = DifferentialDriveWheelVoltages.proto.unpack(proto);
    assertEquals(DATA.getLeft(), data.getLeft());
    assertEquals(DATA.getRight(), data.getRight());
  }
}
