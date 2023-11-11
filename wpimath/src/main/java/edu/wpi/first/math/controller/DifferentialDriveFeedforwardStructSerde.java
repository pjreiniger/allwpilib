// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class DifferentialDriveFeedforwardStructSerde implements Struct<DifferentialDriveFeedforward> {
  @Override
  public Class<DifferentialDriveFeedforward> getTypeClass() {
    return DifferentialDriveFeedforward.class;
  }

  @Override
  public String getTypeString() {
    return "struct:DifferentialDriveFeedforward";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 4;
  }

  @Override
  public String getSchema() {
    return "double kv_linear;double ka_linear;double kv_angular;double ka_angular";
  }

  @Override
  public DifferentialDriveFeedforward unpack(ByteBuffer bb) {
    double kv_linear = bb.getDouble();
    double ka_linear = bb.getDouble();
    double kv_angular = bb.getDouble();
    double ka_angular = bb.getDouble();
    return new DifferentialDriveFeedforward(kv_linear, ka_linear, kv_angular, ka_angular);
  }

  @Override
  public void pack(ByteBuffer bb, DifferentialDriveFeedforward value) {
//    bb.putDouble(value.getKvLinear());
//    bb.putDouble(value.getKaLinear());
//    bb.putDouble(value.getKvAngular());
//    bb.putDouble(value.getKaAngular());
  }
}
