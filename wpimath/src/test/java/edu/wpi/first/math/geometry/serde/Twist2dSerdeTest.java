package edu.wpi.first.math.geometry;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Geometry2D.ProtobufTwist2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class Twist2dSerdeTest {
  private static final Twist2d DATA = new Twist2d(1.91, 2.29, 35.04);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82, 64, 65, -123, 30, -72, 81, -21, -123};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(Twist2d.struct.getSize());
    Twist2d.struct.pack(buffer, DATA);

    byte[] actual = buffer.array();
    System.out.println(Arrays.toString(actual));
    assertArrayEquals(actual, STRUCT_BUFFER);
  }

  @Test
  void testStructUnpack() {
    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);

    Twist2d data = Twist2d.struct.unpack(buffer);
    assertEquals(DATA.dx, data.dx);
    assertEquals(DATA.dy, data.dy);
    assertEquals(DATA.dtheta, data.dtheta);
  }

  @Test
  void testProtoPack() {
    ProtobufTwist2d proto = Twist2d.proto.createMessage();
    Twist2d.proto.pack(proto, DATA);

    assertEquals(DATA.dx, proto.getDxMeters());
    assertEquals(DATA.dy, proto.getDyMeters());
    assertEquals(DATA.dtheta, proto.getDthetaRadians());
  }

  @Test
  void testProtoUnpack() {
    ProtobufTwist2d proto = Twist2d.proto.createMessage();
    proto.setDxMeters(DATA.dx);
    proto.setDyMeters(DATA.dy);
    proto.setDthetaRadians(DATA.dtheta);
    
    Twist2d data = Twist2d.proto.unpack(proto);
    assertEquals(DATA.dx, data.dx);
    assertEquals(DATA.dy, data.dy);
    assertEquals(DATA.dtheta, data.dtheta);
  }
}
