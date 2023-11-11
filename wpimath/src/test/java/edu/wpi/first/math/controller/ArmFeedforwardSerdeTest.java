// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Controller.ProtobufArmFeedforward;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class ArmFeedforwardSerdeTest {
  private static final ArmFeedforward DATA = new ArmFeedforward(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(ArmFeedforward.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    ArmFeedforward.struct.pack(buffer, DATA);

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

    ArmFeedforward data = ArmFeedforward.struct.unpack(buffer);
    assertEquals(DATA.getKs(), data.getKs());
    assertEquals(DATA.getKg(), data.getKg());
    assertEquals(DATA.getKv(), data.getKv());
    assertEquals(DATA.getKa(), data.getKa());
  }

  @Test
  void testProtoPack() {
    ProtobufArmFeedforward proto = ArmFeedforward.proto.createMessage();
    ArmFeedforward.proto.pack(proto, DATA);

    assertEquals(DATA.getKs(), proto.getKs());
    assertEquals(DATA.getKg(), proto.getKg());
    assertEquals(DATA.getKv(), proto.getKv());
    assertEquals(DATA.getKa(), proto.getKa());
  }

  @Test
  void testProtoUnpack() {
    ProtobufArmFeedforward proto = ArmFeedforward.proto.createMessage();
    proto.setKs(DATA.getKs());
    proto.setKg(DATA.getKg());
    proto.setKv(DATA.getKv());
    proto.setKa(DATA.getKa());

    ArmFeedforward data = ArmFeedforward.proto.unpack(proto);
    assertEquals(DATA.getKs(), data.getKs());
    assertEquals(DATA.getKg(), data.getKg());
    assertEquals(DATA.getKv(), data.getKv());
    assertEquals(DATA.getKa(), data.getKa());
  }
}
