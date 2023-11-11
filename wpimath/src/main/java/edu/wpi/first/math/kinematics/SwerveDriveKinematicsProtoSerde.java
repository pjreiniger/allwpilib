// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.math.proto.Kinematics.ProtobufSwerveDriveKinematics;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class SwerveDriveKinematicsProtoSerde implements Protobuf<SwerveDriveKinematics, ProtobufSwerveDriveKinematics> {
  @Override
  public Class<SwerveDriveKinematics> getTypeClass() {
    return SwerveDriveKinematics.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufSwerveDriveKinematics.getDescriptor();
  }

//  @Override
//  public Protobuf<?, ?>[] getNested() {
//    return new Protobuf<?, ?>[] {RepeatedCompositeFieldContainer.proto};
//  }

  @Override
  public ProtobufSwerveDriveKinematics createMessage() {
    return ProtobufSwerveDriveKinematics.newInstance();
  }

  @Override
  public SwerveDriveKinematics unpack(ProtobufSwerveDriveKinematics msg) {
//    return new SwerveDriveKinematics(
//        RepeatedCompositeFieldContainer.proto.unpack(msg.getModules()));
    return null;
  }

  @Override
  public void pack(ProtobufSwerveDriveKinematics msg, SwerveDriveKinematics value) {
//    RepeatedCompositeFieldContainer.proto.pack(msg.getMutableModules(), value.getModules());

  }
}
