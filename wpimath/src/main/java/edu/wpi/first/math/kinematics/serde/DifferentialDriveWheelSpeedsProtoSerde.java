// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics.serde;

import edu.wpi.first.math.kinematics.DifferentialDriveWheelSpeeds;
import edu.wpi.first.math.proto.Kinematics.ProtobufDifferentialDriveWheelSpeeds;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class DifferentialDriveWheelSpeedsProtoSerde
    implements Protobuf<DifferentialDriveWheelSpeeds, ProtobufDifferentialDriveWheelSpeeds> {
  @Override
  public Class<DifferentialDriveWheelSpeeds> getTypeClass() {
    return DifferentialDriveWheelSpeeds.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufDifferentialDriveWheelSpeeds.getDescriptor();
  }

  @Override
  public ProtobufDifferentialDriveWheelSpeeds createMessage() {
    return ProtobufDifferentialDriveWheelSpeeds.newInstance();
  }

  @Override
  public DifferentialDriveWheelSpeeds unpack(ProtobufDifferentialDriveWheelSpeeds msg) {
    return new DifferentialDriveWheelSpeeds(msg.getLeftMps(), msg.getRightMps());
  }

  @Override
  public void pack(ProtobufDifferentialDriveWheelSpeeds msg, DifferentialDriveWheelSpeeds value) {
    msg.setLeftMps(value.leftMetersPerSecond);
    msg.setRightMps(value.rightMetersPerSecond);
  }
}
