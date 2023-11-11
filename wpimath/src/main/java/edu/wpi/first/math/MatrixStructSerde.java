// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math..;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class MatrixStructSerde implements Struct<Matrix> {
  @Override
  public Class<Matrix> getTypeClass() {
    return Matrix.class;
  }

  @Override
  public String getTypeString() {
    return "struct:Matrix";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 3;
  }

  @Override
  public String getSchema() {
    return "int num_rows;int num_cols;double data";
  }

  @Override
  public Matrix unpack(ByteBuffer bb) {
    int num_rows = bb.getInt();
    int num_cols = bb.getInt();
    double data = bb.getDouble();
    return new Matrix(num_rows, num_cols, data);
  }

  @Override
  public void pack(ByteBuffer bb, Matrix value) {
    bb.putInt(value.getNumRows());
    bb.putInt(value.getNumCols());
    bb.putDouble(value.getData());
  }
}
