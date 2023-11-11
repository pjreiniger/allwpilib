// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class DifferentialDriveWheelVoltagesStructSerde implements Struct<DifferentialDriveWheelVoltages> {
  @Override
  public Class<DifferentialDriveWheelVoltages> getTypeClass() {
    return DifferentialDriveWheelVoltages.class;
  }

  @Override
  public String getTypeString() {
    return "struct:DifferentialDriveWheelVoltages";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 2;
  }

  @Override
  public String getSchema() {
    return "double left;double right";
  }

  @Override
  public DifferentialDriveWheelVoltages unpack(ByteBuffer bb) {
    double left = bb.getDouble();
    double right = bb.getDouble();
    return new DifferentialDriveWheelVoltages(left, right);
  }

  @Override
  public void pack(ByteBuffer bb, DifferentialDriveWheelVoltages value) {
    bb.putDouble(value.left);
    bb.putDouble(value.right);
  }
}
