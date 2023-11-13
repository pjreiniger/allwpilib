// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics.serde;

import edu.wpi.first.math.kinematics.DifferentialDriveKinematics;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class DifferentialDriveKinematicsStructSerde implements Struct<DifferentialDriveKinematics> {
  @Override
  public Class<DifferentialDriveKinematics> getTypeClass() {
    return DifferentialDriveKinematics.class;
  }

  @Override
  public String getTypeString() {
    return "struct:DifferentialDriveKinematics";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 1;
  }

  @Override
  public String getSchema() {
    return "double track_width";
  }

  @Override
  public DifferentialDriveKinematics unpack(ByteBuffer bb) {
    double track_width = bb.getDouble();
    return new DifferentialDriveKinematics(track_width);
  }

  @Override
  public void pack(ByteBuffer bb, DifferentialDriveKinematics value) {
    bb.putDouble(value.trackWidthMeters);
  }
}
