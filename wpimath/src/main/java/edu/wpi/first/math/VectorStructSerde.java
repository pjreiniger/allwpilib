// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math..;

import edu.wpi.first.util.struct.Struct;
import java.nio.ByteBuffer;

public class VectorStructSerde implements Struct<Vector> {
  @Override
  public Class<Vector> getTypeClass() {
    return Vector.class;
  }

  @Override
  public String getTypeString() {
    return "struct:Vector";
  }

  @Override
  public int getSize() {
    return kSizeDouble * 1;
  }

  @Override
  public String getSchema() {
    return "double rows";
  }

  @Override
  public Vector unpack(ByteBuffer bb) {
    double rows = bb.getDouble();
    return new Vector(rows);
  }

  @Override
  public void pack(ByteBuffer bb, Vector value) {
    bb.putDouble(value.getRows());
  }
}
