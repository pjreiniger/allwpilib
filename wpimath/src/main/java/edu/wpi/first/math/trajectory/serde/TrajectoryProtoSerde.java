// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.trajectory.serde;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.proto.Trajectory.ProtobufTrajectory;
import edu.wpi.first.math.trajectory.Trajectory;
import edu.wpi.first.util.protobuf.Protobuf;
import java.util.ArrayList;
import java.util.List;
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
    return new Protobuf<?, ?>[] {Trajectory.State.proto};
  }

  @Override
  public ProtobufTrajectory createMessage() {
    return ProtobufTrajectory.newInstance();
  }

  public Trajectory.State unpack(ProtobufTrajectory.State msg) {
    return new Trajectory.State(
        msg.getTimeSeconds(),
        msg.getVelocityMps(),
        msg.getAccelerationMpsSq(),
        Pose2d.proto.unpack(msg.getMutablePose()),
        msg.getCurvatureRadPerMeter());
  }

  public void pack(ProtobufTrajectory.State msg, Trajectory.State value) {
    msg.setTimeSeconds(value.timeSeconds);
    msg.setVelocityMps(value.velocityMetersPerSecond);
    msg.setAccelerationMpsSq(value.accelerationMetersPerSecondSq);
    Pose2d.proto.pack(msg.getMutablePose(), value.poseMeters);
    msg.setCurvatureRadPerMeter(value.curvatureRadPerMeter);
  }

  @Override
  public Trajectory unpack(ProtobufTrajectory msg) {
    List<Trajectory.State> states = new ArrayList<>();
    for (ProtobufTrajectory.State protoState : msg.getStates()) {
      states.add(unpack(protoState));
    }
    return new Trajectory(states);
  }

  @Override
  public void pack(ProtobufTrajectory msg, Trajectory value) {
    for (Trajectory.State state : value.getStates()) {
      ProtobufTrajectory.State protoState = ProtobufTrajectory.State.newInstance();
      pack(protoState, state);
      msg.getMutableStates().add(protoState);
    }
  }
}
