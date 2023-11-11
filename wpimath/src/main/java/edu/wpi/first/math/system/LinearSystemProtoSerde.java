//// Copyright (c) FIRST and other WPILib contributors.
//// Open Source Software; you can modify and/or share it under the terms of
//// the WPILib BSD license file in the root directory of this project.
//
//package edu.wpi.first.math.system;
//
//import edu.wpi.first.math.proto.System.ProtobufLinearSystem;
//import edu.wpi.first.util.protobuf.Protobuf;
//import us.hebi.quickbuf.Descriptors.Descriptor;
//
//public class LinearSystemProtoSerde implements Protobuf<LinearSystem, ProtobufLinearSystem> {
//  @Override
//  public Class<LinearSystem> getTypeClass() {
//    return LinearSystem.class;
//  }
//
//  @Override
//  public Descriptor getDescriptor() {
//    return ProtobufLinearSystem.getDescriptor();
//  }
//
//  @Override
//  public Protobuf<?, ?>[] getNested() {
//    return new Protobuf<?, ?>[] {Matrix.proto, Matrix.proto, Matrix.proto, Matrix.proto};
//  }
//
//  @Override
//  public ProtobufLinearSystem createMessage() {
//    return ProtobufLinearSystem.newInstance();
//  }
//
//  @Override
//  public LinearSystem unpack(ProtobufLinearSystem msg) {
//    return new LinearSystem(msg.getNumStates(), msg.getNumInputs(), msg.getNumOutputs(),
//        Matrix.proto.unpack(msg.getA()),
//        Matrix.proto.unpack(msg.getB()),
//        Matrix.proto.unpack(msg.getC()),
//        Matrix.proto.unpack(msg.getD()));
//  }
//
//  @Override
//  public void pack(ProtobufLinearSystem msg, LinearSystem value) {
//    msg.setNumStates(value.getNumStates());
//    msg.setNumInputs(value.getNumInputs());
//    msg.setNumOutputs(value.getNumOutputs());
//    Matrix.proto.pack(msg.getMutableA(), value.getA());
//    Matrix.proto.pack(msg.getMutableB(), value.getB());
//    Matrix.proto.pack(msg.getMutableC(), value.getC());
//    Matrix.proto.pack(msg.getMutableD(), value.getD());
//
//  }
//}
