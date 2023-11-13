// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.trajectory;

import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.proto.Trajectory.ProtobufTrajectory;
import java.util.List;
import org.junit.jupiter.api.Test;

public class TrajectorySerdeTest {
  private static final Trajectory DATA =
      new Trajectory(
          List.of(
              new Trajectory.State(
                  1.1,
                  2.2,
                  3.3,
                  new Pose2d(new Translation2d(1.2, 3.4), Rotation2d.fromDegrees(1.6)),
                  4.0),
              new Trajectory.State(
                  1.2,
                  2.3,
                  3.4,
                  new Pose2d(new Translation2d(2.2, 4.4), Rotation2d.fromDegrees(3.6)),
                  4.4),
              new Trajectory.State(
                  1.3,
                  2.4,
                  3.5,
                  new Pose2d(new Translation2d(3.2, 5.4), Rotation2d.fromDegrees(5.6)),
                  4.8),
              new Trajectory.State(
                  1.4,
                  2.5,
                  3.6,
                  new Pose2d(new Translation2d(4.2, 6.4), Rotation2d.fromDegrees(7.6)),
                  5.2)));

  @Test
  void testProtoPack() {
    ProtobufTrajectory proto = Trajectory.proto.createMessage();
    Trajectory.proto.pack(proto, DATA);

    assertEquals(DATA.getStates().size(), proto.getStates().length());
    for (int i = 0; i < DATA.getStates().size(); ++i) {
      Trajectory.State state = DATA.getStates().get(i);
      ProtobufTrajectory.State protoState = proto.getStates().get(i);

      assertEquals(state.timeSeconds, protoState.getTimeSeconds());
      assertEquals(state.velocityMetersPerSecond, protoState.getVelocityMps());
      assertEquals(state.accelerationMetersPerSecondSq, protoState.getAccelerationMpsSq());
      assertEquals(state.poseMeters, Pose2d.proto.unpack(protoState.getPose()));
      assertEquals(state.curvatureRadPerMeter, protoState.getCurvatureRadPerMeter());
    }
  }

  @Test
  void testProtoUnpack() {
    ProtobufTrajectory proto = Trajectory.proto.createMessage();

    for (Trajectory.State state : DATA.getStates()) {
      ProtobufTrajectory.State protoState = ProtobufTrajectory.State.newInstance();
      Trajectory.proto.pack(protoState, state);
      proto.getMutableStates().add(protoState);
    }

    Trajectory data = Trajectory.proto.unpack(proto);
    assertEquals(DATA.getStates(), data.getStates());
  }
}
