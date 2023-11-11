package edu.wpi.first.math.geometry;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Geometry2D.ProtobufTranslation2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class Translation2dSerdeTest {
  private static final Translation2d DATA = new Translation2d(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Translation2d.struct.getSize());
    Translation2d.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    System.out.println(Arrays.toString(actual));
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);

    Translation2d data = Translation2d.struct.unpack(buffer);
    assertEquals(DATA.getX(), data.getX());
    assertEquals(DATA.getY(), data.getY());
  }

  @Test
  void testProtoPack() {
    ProtobufTranslation2d proto = Translation2d.proto.createMessage();
    Translation2d.proto.pack(proto, DATA);

    assertEquals(DATA.getX(), proto.getXMeters());
    assertEquals(DATA.getY(), proto.getYMeters());
  }

  @Test
  void testProtoUnpack() {
    ProtobufTranslation2d proto = Translation2d.proto.createMessage();
    proto.setXMeters(DATA.getX());
    proto.setYMeters(DATA.getY());
    
    Translation2d data = Translation2d.proto.unpack(proto);
    assertEquals(DATA.getX(), data.getX());
    assertEquals(DATA.getY(), data.getY());
  }
}
