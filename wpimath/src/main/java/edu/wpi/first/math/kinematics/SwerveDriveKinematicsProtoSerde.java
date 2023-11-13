// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.kinematics;

import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.proto.Geometry2D;
import edu.wpi.first.math.proto.Kinematics.ProtobufSwerveDriveKinematics;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class SwerveDriveKinematicsProtoSerde
    implements Protobuf<SwerveDriveKinematics, ProtobufSwerveDriveKinematics> {
  @Override
  public Class<SwerveDriveKinematics> getTypeClass() {
    return SwerveDriveKinematics.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufSwerveDriveKinematics.getDescriptor();
  }

  @Override
  public Protobuf<?, ?>[] getNested() {
    return new Protobuf<?, ?>[] {Translation2d.proto};
  }

  @Override
  public ProtobufSwerveDriveKinematics createMessage() {
    return ProtobufSwerveDriveKinematics.newInstance();
  }

  @Override
  public SwerveDriveKinematics unpack(ProtobufSwerveDriveKinematics msg) {
    Translation2d[] translations = new Translation2d[msg.getModules().length()];
    for (int i = 0; i < msg.getModules().length(); ++i) {
      translations[i] = Translation2d.proto.unpack(msg.getModules().get(i));
    }
    return new SwerveDriveKinematics(translations);
  }

  @Override
  public void pack(ProtobufSwerveDriveKinematics msg, SwerveDriveKinematics value) {
    Translation2d[] translations = value.getModules();
    for (Translation2d translation : translations) {
      Geometry2D.ProtobufTranslation2d protoTranslation =
          Geometry2D.ProtobufTranslation2d.newInstance();
      Translation2d.proto.pack(protoTranslation, translation);
      msg.addModules(protoTranslation);
    }
  }
}
