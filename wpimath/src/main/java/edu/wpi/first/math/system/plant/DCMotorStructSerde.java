// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.system.plant;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class DCMotorStructSerde implements Struct<DCMotor> {
  @Override
  public Class<DCMotor> getTypeClass() {
    return DCMotor.class;
  }

  @Override
  public String getTypeString() {
    return "struct:DCMotor";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 8;
  }

  @Override
  public String getSchema() {
    return "double nominal_voltage;double stall_torque;double stall_current;double free_current;double free_speed;double r;double kv;double kt";
  }

  @Override
  public DCMotor unpack(ByteBuffer bb) {
    double nominal_voltage = bb.getDouble();
    double stall_torque = bb.getDouble();
    double stall_current = bb.getDouble();
    double free_current = bb.getDouble();
    double free_speed = bb.getDouble();
    double r = bb.getDouble();
    double kv = bb.getDouble();
    double kt = bb.getDouble();
    return null;
//    return new DCMotor(nominal_voltage, stall_torque, stall_current, free_current, free_speed, r, kv, kt);
  }

  @Override
  public void pack(ByteBuffer bb, DCMotor value) {
//    bb.putDouble(value.getNominalVoltage());
//    bb.putDouble(value.getStallTorque());
//    bb.putDouble(value.getStallCurrent());
//    bb.putDouble(value.getFreeCurrent());
//    bb.putDouble(value.getFreeSpeed());
//    bb.putDouble(value.getR());
//    bb.putDouble(value.getKv());
//    bb.putDouble(value.getKt());
  }
}
