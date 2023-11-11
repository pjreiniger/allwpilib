package edu.wpi.first.math.geometry;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Geometry2D.ProtobufRotation2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class Rotation2dSerdeTest {
  private static final Rotation2d DATA = new Rotation2d(35.04);
  private static final byte[] STRUCT_BUFFER = new byte[]{64, 65, -123, 30, -72, 81, -21, -123};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Rotation2d.struct.getSize());
    Rotation2d.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    System.out.println(Arrays.toString(actual));
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);

    Rotation2d data = Rotation2d.struct.unpack(buffer);
    assertEquals(DATA.getRadians(), data.getRadians());
  }

  @Test
  void testProtoPack() {
    ProtobufRotation2d proto = Rotation2d.proto.createMessage();
    Rotation2d.proto.pack(proto, DATA);

    assertEquals(DATA.getRadians(), proto.getRadians());
  }

  @Test
  void testProtoUnpack() {
    ProtobufRotation2d proto = Rotation2d.proto.createMessage();
    proto.setRadians(DATA.getRadians());
    
    Rotation2d data = Rotation2d.proto.unpack(proto);
    assertEquals(DATA.getRadians(), data.getRadians());
  }
}
