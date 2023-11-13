// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.proto.Geometry2D;
import edu.wpi.first.math.proto.Kinematics.ProtobufSwerveDriveKinematics;
import org.junit.jupiter.api.Test;

public class SwerveDriveKinematicsSerdeTest {
  private static final SwerveDriveKinematics DATA =
      new SwerveDriveKinematics(
          new Translation2d(1, 2),
          new Translation2d(3, 4),
          new Translation2d(5, 6),
          new Translation2d(7, 8));

  @Test
  void testProtoPack() {
    ProtobufSwerveDriveKinematics proto = SwerveDriveKinematics.proto.createMessage();
    SwerveDriveKinematics.proto.pack(proto, DATA);

    assertEquals(DATA.getModules().length, proto.getModules().length());
    for (int i = 0; i < DATA.getModules().length; ++i) {
      assertEquals(DATA.getModules()[i].getX(), proto.getModules().get(i).getXMeters());
      assertEquals(DATA.getModules()[i].getY(), proto.getModules().get(i).getYMeters());
    }
  }

  @Test
  void testProtoUnpack() {
    ProtobufSwerveDriveKinematics proto = SwerveDriveKinematics.proto.createMessage();
    for (Translation2d translation : DATA.getModules()) {
      Geometry2D.ProtobufTranslation2d protoTranslation =
          Geometry2D.ProtobufTranslation2d.newInstance();
      Translation2d.proto.pack(protoTranslation, translation);
      proto.addModules(protoTranslation);
    }

    SwerveDriveKinematics data = SwerveDriveKinematics.proto.unpack(proto);
    assertArrayEquals(DATA.getModules(), data.getModules());
  }
}
