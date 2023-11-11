// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import edu.wpi.first.math.proto.Controller.ProtobufSimpleMotorFeedforward;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class SimpleMotorFeedforwardProtoSerde implements Protobuf<SimpleMotorFeedforward, ProtobufSimpleMotorFeedforward> {
  @Override
  public Class<SimpleMotorFeedforward> getTypeClass() {
    return SimpleMotorFeedforward.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufSimpleMotorFeedforward.getDescriptor();
  }

  @Override
  public ProtobufSimpleMotorFeedforward createMessage() {
    return ProtobufSimpleMotorFeedforward.newInstance();
  }

  @Override
  public SimpleMotorFeedforward unpack(ProtobufSimpleMotorFeedforward msg) {
    return new SimpleMotorFeedforward(msg.getKs(), msg.getKv(), msg.getKa());
  }

  @Override
  public void pack(ProtobufSimpleMotorFeedforward msg, SimpleMotorFeedforward value) {
    msg.setKs(value.ks);
    msg.setKv(value.kv);
    msg.setKa(value.ka);

  }
}
