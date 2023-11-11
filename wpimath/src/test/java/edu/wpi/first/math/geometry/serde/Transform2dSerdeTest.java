package edu.wpi.first.math.geometry;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Geometry2D.ProtobufTransform2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class Transform2dSerdeTest {
  private static final Transform2d DATA = new Transform2d(new Translation2d(1.91, 2.29), Rotation2d.fromDegrees(35.04));
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82, 63, -29, -111, -19, 88, 36, -77, -41};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Transform2d.struct.getSize());
    Transform2d.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    System.out.println(Arrays.toString(actual));
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);

    Transform2d data = Transform2d.struct.unpack(buffer);
    assertEquals(DATA.getTranslation(), data.getTranslation());
    assertEquals(DATA.getRotation(), data.getRotation());
  }

  @Test
  void testProtoPack() {
    ProtobufTransform2d proto = Transform2d.proto.createMessage();
    Transform2d.proto.pack(proto, DATA);

    assertEquals(DATA.getTranslation(), Translation2d.proto.unpack(proto.getTranslation()));
    assertEquals(DATA.getRotation(), Rotation2d.proto.unpack(proto.getRotation()));
  }

  @Test
  void testProtoUnpack() {
    ProtobufTransform2d proto = Transform2d.proto.createMessage();
    Translation2d.proto.pack(proto.getMutableTranslation(), DATA.getTranslation());
    Rotation2d.proto.pack(proto.getMutableRotation(), DATA.getRotation());
    
    Transform2d data = Transform2d.proto.unpack(proto);
    assertEquals(DATA.getTranslation(), data.getTranslation());
    assertEquals(DATA.getRotation(), data.getRotation());
  }
}
