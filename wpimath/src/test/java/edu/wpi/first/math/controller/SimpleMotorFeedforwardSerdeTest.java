// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Controller.ProtobufSimpleMotorFeedforward;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class SimpleMotorFeedforwardSerdeTest {
  private static final SimpleMotorFeedforward DATA = new SimpleMotorFeedforward(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{-113, -62, -11, 40, 92, -113, -2, 63, 82, -72, 30, -123, -21, 81, 2, 64, 0, 0, 0, 0, 0, 0, 0, 0};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(SimpleMotorFeedforward.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    SimpleMotorFeedforward.struct.pack(buffer, DATA);

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

    SimpleMotorFeedforward data = SimpleMotorFeedforward.struct.unpack(buffer);
    assertEquals(DATA.ks, data.ks);
    assertEquals(DATA.kv, data.kv);
    assertEquals(DATA.ka, data.ka);
  }

  @Test
  void testProtoPack() {
    ProtobufSimpleMotorFeedforward proto = SimpleMotorFeedforward.proto.createMessage();
    SimpleMotorFeedforward.proto.pack(proto, DATA);

    assertEquals(DATA.ks, proto.getKs());
    assertEquals(DATA.kv, proto.getKv());
    assertEquals(DATA.ka, proto.getKa());
  }

  @Test
  void testProtoUnpack() {
    ProtobufSimpleMotorFeedforward proto = SimpleMotorFeedforward.proto.createMessage();
    proto.setKs(DATA.ks);
    proto.setKv(DATA.kv);
    proto.setKa(DATA.ka);

    SimpleMotorFeedforward data = SimpleMotorFeedforward.proto.unpack(proto);
    assertEquals(DATA.ks, data.ks);
    assertEquals(DATA.kv, data.kv);
    assertEquals(DATA.ka, data.ka);
  }
}
