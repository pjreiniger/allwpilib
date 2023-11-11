// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.proto.Kinematics.ProtobufMecanumDriveWheelPositions;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import org.junit.jupiter.api.Test;
import java.util.Arrays;

public class MecanumDriveWheelPositionsSerdeTest {
  private static final MecanumDriveWheelPositions DATA = new MecanumDriveWheelPositions(1.91, 2.29);
  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};

  @Test
  void testStructPack() {
    ByteBuffer buffer = ByteBuffer.allocate(MecanumDriveWheelPositions.struct.getSize());
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    MecanumDriveWheelPositions.struct.pack(buffer, DATA);

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

    MecanumDriveWheelPositions data = MecanumDriveWheelPositions.struct.unpack(buffer);
    assertEquals(DATA.getFrontLeftMeters(), data.getFrontLeftMeters());
    assertEquals(DATA.getFrontRightMeters(), data.getFrontRightMeters());
    assertEquals(DATA.getRearLeftMeters(), data.getRearLeftMeters());
    assertEquals(DATA.getRearRightMeters(), data.getRearRightMeters());
  }

  @Test
  void testProtoPack() {
    ProtobufMecanumDriveWheelPositions proto = MecanumDriveWheelPositions.proto.createMessage();
    MecanumDriveWheelPositions.proto.pack(proto, DATA);

    assertEquals(DATA.getFrontLeftMeters(), proto.getFrontLeftMeters());
    assertEquals(DATA.getFrontRightMeters(), proto.getFrontRightMeters());
    assertEquals(DATA.getRearLeftMeters(), proto.getRearLeftMeters());
    assertEquals(DATA.getRearRightMeters(), proto.getRearRightMeters());
  }

  @Test
  void testProtoUnpack() {
    ProtobufMecanumDriveWheelPositions proto = MecanumDriveWheelPositions.proto.createMessage();
    proto.setFrontLeftMeters(DATA.getFrontLeftMeters());
    proto.setFrontRightMeters(DATA.getFrontRightMeters());
    proto.setRearLeftMeters(DATA.getRearLeftMeters());
    proto.setRearRightMeters(DATA.getRearRightMeters());

    MecanumDriveWheelPositions data = MecanumDriveWheelPositions.proto.unpack(proto);
    assertEquals(DATA.getFrontLeftMeters(), data.getFrontLeftMeters());
    assertEquals(DATA.getFrontRightMeters(), data.getFrontRightMeters());
    assertEquals(DATA.getRearLeftMeters(), data.getRearLeftMeters());
    assertEquals(DATA.getRearRightMeters(), data.getRearRightMeters());
  }
}
