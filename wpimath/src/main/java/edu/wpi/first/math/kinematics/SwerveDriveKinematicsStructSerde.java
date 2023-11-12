// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class SwerveDriveKinematicsStructSerde implements Struct<SwerveDriveKinematics> {
  @Override
  public Class<SwerveDriveKinematics> getTypeClass() {
    return SwerveDriveKinematics.class;
  }

  @Override
  public String getTypeString() {
    return "struct:SwerveDriveKinematics";
  }

  @Override
  public int getSize() {
    return 0;
    // return RepeatedCompositeFieldContainer.struct.getSize();
  }

  @Override
  public String getSchema() {
    return "RepeatedCompositeFieldContainer modules";
  }

  // @Override
  // public Struct<?>[] getNested() {
  //   return new Struct<?>[] {SwerveModuleState.proto, SwerveModuleState.proto, SwerveModuleState.proto, SwerveModuleState.proto};
  // }

  @Override
  public SwerveDriveKinematics unpack(ByteBuffer bb) {
    // RepeatedCompositeFieldContainer modules = RepeatedCompositeFieldContainer.struct.unpack(bb);
    // return new SwerveDriveKinematics(modules);
    return null;
  }

  @Override
  public void pack(ByteBuffer bb, SwerveDriveKinematics value) {
    // RepeatedCompositeFieldContainer.struct.pack(bb, value.getModules());
  }
}
