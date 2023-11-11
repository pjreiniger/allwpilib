//// Copyright (c) FIRST and other WPILib contributors.
//// Open Source Software; you can modify and/or share it under the terms of
//// the WPILib BSD license file in the root directory of this project.
//
//package edu.wpi.first.math..;
//
//import edu.wpi.first.math.proto.Wpimath.ProtobufVector;
//import edu.wpi.first.util.protobuf.Protobuf;
//import us.hebi.quickbuf.Descriptors.Descriptor;
//
//public class VectorProtoSerde implements Protobuf<Vector, ProtobufVector> {
//  @Override
//  public Class<Vector> getTypeClass() {
//    return Vector.class;
//  }
//
//  @Override
//  public Descriptor getDescriptor() {
//    return ProtobufVector.getDescriptor();
//  }
//
//  @Override
//  public ProtobufVector createMessage() {
//    return ProtobufVector.newInstance();
//  }
//
//  @Override
//  public Vector unpack(ProtobufVector msg) {
//    return new Vector(msg.getRows());
//  }
//
//  @Override
//  public void pack(ProtobufVector msg, Vector value) {
////    msg.setRows(value.getRows());
//
//  }
//}
