// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Controller.ProtobufArmFeedforward;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;

public class ArmFeedforwardSerdeTest {
  private static final ArmFeedforward DATA = new ArmFeedforward(1.91, 2.29, 35.04, 1.74);
  private static final byte[] STRUCT_BUFFER =
      new byte[] {
        -113, -62, -11, 40, 92, -113, -2, 63, 82, -72, 30, -123, -21, 81, 2, 64, -123, -21, 81, -72,
        30, -123, 65, 64, -41, -93, 112, 61, 10, -41, -5, 63
      };

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(ArmFeedforward.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    ArmFeedforward.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);
    buffer.order(ByteOrder.LITTLE_ENDIAN);

    ArmFeedforward data = ArmFeedforward.struct.unpack(buffer);
    assertEquals(DATA.ks, data.ks);
    assertEquals(DATA.kg, data.kg);
    assertEquals(DATA.kv, data.kv);
    assertEquals(DATA.ka, data.ka);
  }

  @Test
  void testProtoPack() {
    ProtobufArmFeedforward proto = ArmFeedforward.proto.createMessage();
    ArmFeedforward.proto.pack(proto, DATA);

    assertEquals(DATA.ks, proto.getKs());
    assertEquals(DATA.kg, proto.getKg());
    assertEquals(DATA.kv, proto.getKv());
    assertEquals(DATA.ka, proto.getKa());
  }

  @Test
  void testProtoUnpack() {
    ProtobufArmFeedforward proto = ArmFeedforward.proto.createMessage();
    proto.setKs(DATA.ks);
    proto.setKg(DATA.kg);
    proto.setKv(DATA.kv);
    proto.setKa(DATA.ka);

    ArmFeedforward data = ArmFeedforward.proto.unpack(proto);
    assertEquals(DATA.ks, data.ks);
    assertEquals(DATA.kg, data.kg);
    assertEquals(DATA.kv, data.kv);
    assertEquals(DATA.ka, data.ka);
  }
}
