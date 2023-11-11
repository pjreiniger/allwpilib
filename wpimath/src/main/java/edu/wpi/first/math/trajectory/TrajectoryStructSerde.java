// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.trajectory;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class TrajectoryStructSerde implements Struct<Trajectory> {
  @Override
  public Class<Trajectory> getTypeClass() {
    return Trajectory.class;
  }

  @Override
  public String getTypeString() {
    return "struct:Trajectory";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 1+ RepeatedCompositeFieldContainer.struct.getSize();
  }

  @Override
  public String getSchema() {
    return "double total_time;RepeatedCompositeFieldContainer states";
  }

  @Override
  public Struct<?>[] getNested() {
    return new Struct<?>[] {RepeatedCompositeFieldContainer.struct};
  }

  @Override
  public Trajectory unpack(ByteBuffer bb) {
    double total_time = bb.getDouble();
    RepeatedCompositeFieldContainer states = RepeatedCompositeFieldContainer.struct.unpack(bb);
    return new Trajectory(total_time, states);
  }

  @Override
  public void pack(ByteBuffer bb, Trajectory value) {
    bb.putDouble(value.getTotalTime());
    RepeatedCompositeFieldContainer.struct.pack(bb, value.getStates());
  }
}
