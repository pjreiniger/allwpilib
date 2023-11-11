// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class MecanumDriveKinematicsStructSerde implements Struct<MecanumDriveKinematics> {
  @Override
  public Class<MecanumDriveKinematics> getTypeClass() {
    return MecanumDriveKinematics.class;
  }

  @Override
  public String getTypeString() {
    return "struct:MecanumDriveKinematics";
  }

  @Override
  public int getSize() {
    return Translation2d.struct.getSize() + Translation2d.struct.getSize() + Translation2d.struct.getSize() + Translation2d.struct.getSize();
  }

  @Override
  public String getSchema() {
    return "Translation2d front_left;Translation2d front_right;Translation2d rear_left;Translation2d rear_right";
  }

  @Override
  public Struct<?>[] getNested() {
    return new Struct<?>[] {Translation2d.struct, Translation2d.struct, Translation2d.struct, Translation2d.struct};
  }

  @Override
  public MecanumDriveKinematics unpack(ByteBuffer bb) {
    Translation2d front_left = Translation2d.struct.unpack(bb);
    Translation2d front_right = Translation2d.struct.unpack(bb);
    Translation2d rear_left = Translation2d.struct.unpack(bb);
    Translation2d rear_right = Translation2d.struct.unpack(bb);
    return new MecanumDriveKinematics(front_left, front_right, rear_left, rear_right);
  }

  @Override
  public void pack(ByteBuffer bb, MecanumDriveKinematics value) {
//    Translation2d.struct.pack(bb, value.getFrontLeft());
//    Translation2d.struct.pack(bb, value.getFrontRight());
//    Translation2d.struct.pack(bb, value.getRearLeft());
//    Translation2d.struct.pack(bb, value.getRearRight());
  }
}
