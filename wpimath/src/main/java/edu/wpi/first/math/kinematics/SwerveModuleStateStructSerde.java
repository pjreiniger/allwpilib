// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class SwerveModuleStateStructSerde implements Struct<SwerveModuleState> {
  @Override
  public Class<SwerveModuleState> getTypeClass() {
    return SwerveModuleState.class;
  }

  @Override
  public String getTypeString() {
    return "struct:SwerveModuleState";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 1+ Rotation2d.struct.getSize();
  }

  @Override
  public String getSchema() {
    return "double speed_mps;Rotation2d angle";
  }

  @Override
  public Struct<?>[] getNested() {
    return new Struct<?>[] {Rotation2d.struct};
  }

  @Override
  public SwerveModuleState unpack(ByteBuffer bb) {
    double speed_mps = bb.getDouble();
    Rotation2d angle = Rotation2d.struct.unpack(bb);
    return new SwerveModuleState(speed_mps, angle);
  }

  @Override
  public void pack(ByteBuffer bb, SwerveModuleState value) {
    bb.putDouble(value.speedMetersPerSecond);
    Rotation2d.struct.pack(bb, value.angle);
  }
}
