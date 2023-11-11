// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.system.plant;

import edu.wpi.first.math.proto.Plant.ProtobufDCMotor;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class DCMotorProtoSerde implements Protobuf<DCMotor, ProtobufDCMotor> {
  @Override
  public Class<DCMotor> getTypeClass() {
    return DCMotor.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufDCMotor.getDescriptor();
  }

  @Override
  public ProtobufDCMotor createMessage() {
    return ProtobufDCMotor.newInstance();
  }

  @Override
  public DCMotor unpack(ProtobufDCMotor msg) {
    return null;
//    return new DCMotor(msg.getNominalVoltage(), msg.getStallTorque(), msg.getStallCurrent(), msg.getFreeCurrent(), msg.getFreeSpeed(), msg.getR(), msg.getKv(), msg.getKt());
  }

  @Override
  public void pack(ProtobufDCMotor msg, DCMotor value) {
//    msg.setNominalVoltage(value.getNominalVoltage());
//    msg.setStallTorque(value.getStallTorque());
//    msg.setStallCurrent(value.getStallCurrent());
//    msg.setFreeCurrent(value.getFreeCurrent());
//    msg.setFreeSpeed(value.getFreeSpeed());
//    msg.setR(value.getR());
//    msg.setKv(value.getKv());
//    msg.setKt(value.getKt());

  }
}
