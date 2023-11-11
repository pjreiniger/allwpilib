package edu.wpi.first.math.geometry;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Geometry2D.ProtobufPose2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class Pose2dSerdeTest {
  private static final Pose2d DATA = new Pose2d(new Translation2d(1.91, 2.29), Rotation2d.fromDegrees(35.04));
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82, 63, -29, -111, -19, 88, 36, -77, -41};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Pose2d.struct.getSize());
    Pose2d.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    String newContent = new String(buffer.array());
    System.out.println(Arrays.toString(actual));
    System.out.println(newContent);
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);

    Pose2d data = Pose2d.struct.unpack(buffer);
    assertEquals(DATA.getTranslation(), data.getTranslation());
    assertEquals(DATA.getRotation(), data.getRotation());
  }

  @Test
  void testProtoPack() {
    ProtobufPose2d proto = Pose2d.proto.createMessage();
    Pose2d.proto.pack(proto, DATA);

    assertEquals(DATA.getTranslation(), Translation2d.proto.unpack(proto.getTranslation()));
    assertEquals(DATA.getRotation(), Rotation2d.proto.unpack(proto.getRotation()));
  }

  @Test
  void testProtoUnpack() {
    ProtobufPose2d proto = Pose2d.proto.createMessage();
    Translation2d.proto.pack(proto.getMutableTranslation(), DATA.getTranslation());
    Rotation2d.proto.pack(proto.getMutableRotation(), DATA.getRotation());
    
    Pose2d data = Pose2d.proto.unpack(proto);
    assertEquals(DATA.getTranslation(), data.getTranslation());
    assertEquals(DATA.getRotation(), data.getRotation());
  }
}
