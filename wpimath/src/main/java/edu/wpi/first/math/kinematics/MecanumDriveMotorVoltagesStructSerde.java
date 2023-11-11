// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class MecanumDriveMotorVoltagesStructSerde implements Struct<MecanumDriveMotorVoltages> {
  @Override
  public Class<MecanumDriveMotorVoltages> getTypeClass() {
    return MecanumDriveMotorVoltages.class;
  }

  @Override
  public String getTypeString() {
    return "struct:MecanumDriveMotorVoltages";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 4;
  }

  @Override
  public String getSchema() {
    return "double front_left_volts;double front_right_volts;double rear_left_volts;double rear_right_volts";
  }

  @Override
  public MecanumDriveMotorVoltages unpack(ByteBuffer bb) {
    double front_left_volts = bb.getDouble();
    double front_right_volts = bb.getDouble();
    double rear_left_volts = bb.getDouble();
    double rear_right_volts = bb.getDouble();
    return new MecanumDriveMotorVoltages(front_left_volts, front_right_volts, rear_left_volts, rear_right_volts);
  }

  @Override
  public void pack(ByteBuffer bb, MecanumDriveMotorVoltages value) {
    bb.putDouble(value.getFrontLeftVolts());
    bb.putDouble(value.getFrontRightVolts());
    bb.putDouble(value.getRearLeftVolts());
    bb.putDouble(value.getRearRightVolts());
  }
}
