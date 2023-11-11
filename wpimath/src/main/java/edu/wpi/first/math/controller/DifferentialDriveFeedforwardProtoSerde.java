// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import edu.wpi.first.math.proto.Controller.ProtobufDifferentialDriveFeedforward;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class DifferentialDriveFeedforwardProtoSerde implements Protobuf<DifferentialDriveFeedforward, ProtobufDifferentialDriveFeedforward> {
  @Override
  public Class<DifferentialDriveFeedforward> getTypeClass() {
    return DifferentialDriveFeedforward.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufDifferentialDriveFeedforward.getDescriptor();
  }

  @Override
  public ProtobufDifferentialDriveFeedforward createMessage() {
    return ProtobufDifferentialDriveFeedforward.newInstance();
  }

  @Override
  public DifferentialDriveFeedforward unpack(ProtobufDifferentialDriveFeedforward msg) {
    return new DifferentialDriveFeedforward(msg.getKvLinear(), msg.getKaLinear(), msg.getKvAngular(), msg.getKaAngular());
  }

  @Override
  public void pack(ProtobufDifferentialDriveFeedforward msg, DifferentialDriveFeedforward value) {
//    msg.setKvLinear(value.());
//    msg.setKaLinear(value.getKaLinear());
//    msg.setKvAngular(value.getKvAngular());
//    msg.setKaAngular(value.getKaAngular());

  }
}
