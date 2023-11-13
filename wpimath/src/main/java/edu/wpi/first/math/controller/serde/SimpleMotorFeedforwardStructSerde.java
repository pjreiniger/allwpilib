// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller.serde;

import edu.wpi.first.math.controller.SimpleMotorFeedforward;
import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class SimpleMotorFeedforwardStructSerde implements Struct<SimpleMotorFeedforward> {
  @Override
  public Class<SimpleMotorFeedforward> getTypeClass() {
    return SimpleMotorFeedforward.class;
  }

  @Override
  public String getTypeString() {
    return "struct:SimpleMotorFeedforward";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 3;
  }

  @Override
  public String getSchema() {
    return "double ks;double kv;double ka";
  }

  @Override
  public SimpleMotorFeedforward unpack(ByteBuffer bb) {
    double ks = bb.getDouble();
    double kv = bb.getDouble();
    double ka = bb.getDouble();
    return new SimpleMotorFeedforward(ks, kv, ka);
  }

  @Override
  public void pack(ByteBuffer bb, SimpleMotorFeedforward value) {
    bb.putDouble(value.ks);
    bb.putDouble(value.kv);
    bb.putDouble(value.ka);
  }
}
