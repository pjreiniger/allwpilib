// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics.serde;

import edu.wpi.first.math.kinematics.MecanumDriveWheelSpeeds;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class MecanumDriveWheelSpeedsStructSerde implements Struct<MecanumDriveWheelSpeeds> {
  @Override
  public Class<MecanumDriveWheelSpeeds> getTypeClass() {
    return MecanumDriveWheelSpeeds.class;
  }

  @Override
  public String getTypeString() {
    return "struct:MecanumDriveWheelSpeeds";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 4;
  }

  @Override
  public String getSchema() {
    return "double front_left;double front_right;double rear_left;double rear_right";
  }

  @Override
  public MecanumDriveWheelSpeeds unpack(ByteBuffer bb) {
    double front_left = bb.getDouble();
    double front_right = bb.getDouble();
    double rear_left = bb.getDouble();
    double rear_right = bb.getDouble();
    return new MecanumDriveWheelSpeeds(front_left, front_right, rear_left, rear_right);
  }

  @Override
  public void pack(ByteBuffer bb, MecanumDriveWheelSpeeds value) {
    bb.putDouble(value.frontLeftMetersPerSecond);
    bb.putDouble(value.frontRightMetersPerSecond);
    bb.putDouble(value.rearLeftMetersPerSecond);
    bb.putDouble(value.rearRightMetersPerSecond);
  }
}