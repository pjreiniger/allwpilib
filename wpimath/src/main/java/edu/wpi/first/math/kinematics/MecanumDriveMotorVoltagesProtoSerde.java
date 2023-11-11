// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.math.proto.Kinematics.ProtobufMecanumDriveMotorVoltages;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class MecanumDriveMotorVoltagesProtoSerde implements Protobuf<MecanumDriveMotorVoltages, ProtobufMecanumDriveMotorVoltages> {
  @Override
  public Class<MecanumDriveMotorVoltages> getTypeClass() {
    return MecanumDriveMotorVoltages.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufMecanumDriveMotorVoltages.getDescriptor();
  }

  @Override
  public ProtobufMecanumDriveMotorVoltages createMessage() {
    return ProtobufMecanumDriveMotorVoltages.newInstance();
  }

  @Override
  public MecanumDriveMotorVoltages unpack(ProtobufMecanumDriveMotorVoltages msg) {
    return new MecanumDriveMotorVoltages(msg.getFrontLeftVolts(), msg.getFrontRightVolts(), msg.getRearLeftVolts(), msg.getRearRightVolts());
  }

  @Override
  public void pack(ProtobufMecanumDriveMotorVoltages msg, MecanumDriveMotorVoltages value) {
    msg.setFrontLeftVolts(value.getFrontLeftVolts());
    msg.setFrontRightVolts(value.getFrontRightVolts());
    msg.setRearLeftVolts(value.getRearLeftVolts());
    msg.setRearRightVolts(value.getRearRightVolts());

  }
}
