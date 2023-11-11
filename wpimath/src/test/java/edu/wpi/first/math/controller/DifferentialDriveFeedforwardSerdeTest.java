// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Controller.ProtobufDifferentialDriveFeedforward;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class DifferentialDriveFeedforwardSerdeTest {
  private static final DifferentialDriveFeedforward DATA = new DifferentialDriveFeedforward(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(DifferentialDriveFeedforward.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    DifferentialDriveFeedforward.struct.pack(buffer, DATA);

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

    DifferentialDriveFeedforward data = DifferentialDriveFeedforward.struct.unpack(buffer);
    assertEquals(DATA.getKvLinear(), data.getKvLinear());
    assertEquals(DATA.getKaLinear(), data.getKaLinear());
    assertEquals(DATA.getKvAngular(), data.getKvAngular());
    assertEquals(DATA.getKaAngular(), data.getKaAngular());
  }

  @Test
  void testProtoPack() {
    ProtobufDifferentialDriveFeedforward proto = DifferentialDriveFeedforward.proto.createMessage();
    DifferentialDriveFeedforward.proto.pack(proto, DATA);

    assertEquals(DATA.getKvLinear(), proto.getKvLinear());
    assertEquals(DATA.getKaLinear(), proto.getKaLinear());
    assertEquals(DATA.getKvAngular(), proto.getKvAngular());
    assertEquals(DATA.getKaAngular(), proto.getKaAngular());
  }

  @Test
  void testProtoUnpack() {
    ProtobufDifferentialDriveFeedforward proto = DifferentialDriveFeedforward.proto.createMessage();
    proto.setKvLinear(DATA.getKvLinear());
    proto.setKaLinear(DATA.getKaLinear());
    proto.setKvAngular(DATA.getKvAngular());
    proto.setKaAngular(DATA.getKaAngular());

    DifferentialDriveFeedforward data = DifferentialDriveFeedforward.proto.unpack(proto);
    assertEquals(DATA.getKvLinear(), data.getKvLinear());
    assertEquals(DATA.getKaLinear(), data.getKaLinear());
    assertEquals(DATA.getKvAngular(), data.getKvAngular());
    assertEquals(DATA.getKaAngular(), data.getKaAngular());
  }
}
