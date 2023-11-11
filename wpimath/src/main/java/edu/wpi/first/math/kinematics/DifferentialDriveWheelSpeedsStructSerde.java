// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class DifferentialDriveWheelSpeedsStructSerde implements Struct<DifferentialDriveWheelSpeeds> {
  @Override
  public Class<DifferentialDriveWheelSpeeds> getTypeClass() {
    return DifferentialDriveWheelSpeeds.class;
  }

  @Override
  public String getTypeString() {
    return "struct:DifferentialDriveWheelSpeeds";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 2;
  }

  @Override
  public String getSchema() {
    return "double left_mps;double right_mps";
  }

  @Override
  public DifferentialDriveWheelSpeeds unpack(ByteBuffer bb) {
    double left_mps = bb.getDouble();
    double right_mps = bb.getDouble();
    return new DifferentialDriveWheelSpeeds(left_mps, right_mps);
  }

  @Override
  public void pack(ByteBuffer bb, DifferentialDriveWheelSpeeds value) {
    bb.putDouble(value.getLeftMps());
    bb.putDouble(value.getRightMps());
  }
}
