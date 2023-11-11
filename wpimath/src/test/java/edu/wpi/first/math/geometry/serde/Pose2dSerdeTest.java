package edu.wpi.first.math.geometry.serde;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.proto.Geometry2D.ProtobufPose2d;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;

public class Pose2dSerdeTest {
  private static final Pose2d DATA =
      new Pose2d(new Translation2d(1.91, 2.29), Rotation2d.fromDegrees(35.04));
  private static final byte[] STRUCT_BUFFER =
      new byte[] {
        -113, -62, -11, 40, 92, -113, -2, 63, 82, -72, 30, -123, -21, 81, 2, 64, -41, -77, 36, 88,
        -19, -111, -29, 63
      };

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Pose2d.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    Pose2d.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);
    buffer.order(ByteOrder.LITTLE_ENDIAN);

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
