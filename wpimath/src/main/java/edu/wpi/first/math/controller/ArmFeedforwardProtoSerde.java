// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.controller;

import edu.wpi.first.math.proto.Controller.ProtobufArmFeedforward;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class ArmFeedforwardProtoSerde implements Protobuf<ArmFeedforward, ProtobufArmFeedforward> {
  @Override
  public Class<ArmFeedforward> getTypeClass() {
    return ArmFeedforward.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufArmFeedforward.getDescriptor();
  }

  @Override
  public ProtobufArmFeedforward createMessage() {
    return ProtobufArmFeedforward.newInstance();
  }

  @Override
  public ArmFeedforward unpack(ProtobufArmFeedforward msg) {
    return new ArmFeedforward(msg.getKs(), msg.getKg(), msg.getKv(), msg.getKa());
  }

  @Override
  public void pack(ProtobufArmFeedforward msg, ArmFeedforward value) {
    msg.setKs(value.ks);
    msg.setKg(value.kg);
    msg.setKv(value.kv);
    msg.setKa(value.ka);

  }
}
