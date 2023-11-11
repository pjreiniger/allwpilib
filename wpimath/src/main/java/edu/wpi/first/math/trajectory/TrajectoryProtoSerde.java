// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.trajectory;

import edu.wpi.first.math.proto.Trajectory.ProtobufTrajectory;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class TrajectoryProtoSerde implements Protobuf<Trajectory, ProtobufTrajectory> {
  @Override
  public Class<Trajectory> getTypeClass() {
    return Trajectory.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufTrajectory.getDescriptor();
  }

  @Override
  public Protobuf<?, ?>[] getNested() {
    return new Protobuf<?, ?>[] {RepeatedCompositeFieldContainer.proto};
  }

  @Override
  public ProtobufTrajectory createMessage() {
    return ProtobufTrajectory.newInstance();
  }

  @Override
  public Trajectory unpack(ProtobufTrajectory msg) {
    return new Trajectory(msg.getTotalTime(), 
        RepeatedCompositeFieldContainer.proto.unpack(msg.getStates()));
  }

  @Override
  public void pack(ProtobufTrajectory msg, Trajectory value) {
    msg.setTotalTime(value.getTotalTime());
    RepeatedCompositeFieldContainer.proto.pack(msg.getMutableStates(), value.getStates());

  }
}
