// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math..;

import edu.wpi.first.math.proto.Wpimath.ProtobufMatrix;
import edu.wpi.first.util.protobuf.Protobuf;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class MatrixProtoSerde implements Protobuf<Matrix, ProtobufMatrix> {
  @Override
  public Class<Matrix> getTypeClass() {
    return Matrix.class;
  }

  @Override
  public Descriptor getDescriptor() {
    return ProtobufMatrix.getDescriptor();
  }

  @Override
  public ProtobufMatrix createMessage() {
    return ProtobufMatrix.newInstance();
  }

  @Override
  public Matrix unpack(ProtobufMatrix msg) {
    return new Matrix(msg.getNumRows(), msg.getNumCols(), msg.getData());
  }

  @Override
  public void pack(ProtobufMatrix msg, Matrix value) {
    msg.setNumRows(value.getNumRows());
    msg.setNumCols(value.getNumCols());
    msg.setData(value.getData());

  }
}
