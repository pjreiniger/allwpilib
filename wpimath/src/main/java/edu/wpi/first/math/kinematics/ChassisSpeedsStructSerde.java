// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class ChassisSpeedsStructSerde implements Struct<ChassisSpeeds> {
  @Override
  public Class<ChassisSpeeds> getTypeClass() {
    return ChassisSpeeds.class;
  }

  @Override
  public String getTypeString() {
    return "struct:ChassisSpeeds";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 3;
  }

  @Override
  public String getSchema() {
    return "double vx_mps;double vy_mps;double omega_rps";
  }

  @Override
  public ChassisSpeeds unpack(ByteBuffer bb) {
    double vx_mps = bb.getDouble();
    double vy_mps = bb.getDouble();
    double omega_rps = bb.getDouble();
    return new ChassisSpeeds(vx_mps, vy_mps, omega_rps);
  }

  @Override
  public void pack(ByteBuffer bb, ChassisSpeeds value) {
    bb.putDouble(value.getVxMps());
    bb.putDouble(value.getVyMps());
    bb.putDouble(value.getOmegaRps());
  }
}
