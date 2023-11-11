//// Copyright (c) FIRST and other WPILib contributors.
//// Open Source Software; you can modify and/or share it under the terms of
//// the WPILib BSD license file in the root directory of this project.
//
//package edu.wpi.first.math.system;
//
//import edu.wpi.first.util.struct.Struct;
//import java.nio.ByteBuffer;
//
//public class LinearSystemStructSerde implements Struct<LinearSystem> {
//  @Override
//  public Class<LinearSystem> getTypeClass() {
//    return LinearSystem.class;
//  }
//
//  @Override
//  public String getTypeString() {
//    return "struct:LinearSystem";
//  }
//
//  @Override
//  public int getSize() {
//    return kSizeDouble * 3+ Matrix.struct.getSize() + Matrix.struct.getSize() + Matrix.struct.getSize() + Matrix.struct.getSize();
//  }
//
//  @Override
//  public String getSchema() {
//    return "int num_states;int num_inputs;int num_outputs;Matrix a;Matrix b;Matrix c;Matrix d";
//  }
//
//  @Override
//  public Struct<?>[] getNested() {
//    return new Struct<?>[] {Matrix.struct, Matrix.struct, Matrix.struct, Matrix.struct};
//  }
//
//  @Override
//  public LinearSystem unpack(ByteBuffer bb) {
//    int num_states = bb.getInt();
//    int num_inputs = bb.getInt();
//    int num_outputs = bb.getInt();
//    Matrix a = Matrix.struct.unpack(bb);
//    Matrix b = Matrix.struct.unpack(bb);
//    Matrix c = Matrix.struct.unpack(bb);
//    Matrix d = Matrix.struct.unpack(bb);
//    return new LinearSystem(num_states, num_inputs, num_outputs, a, b, c, d);
//  }
//
//  @Override
//  public void pack(ByteBuffer bb, LinearSystem value) {
//    bb.putInt(value.getNumStates());
//    bb.putInt(value.getNumInputs());
//    bb.putInt(value.getNumOutputs());
//    Matrix.struct.pack(bb, value.getA());
//    Matrix.struct.pack(bb, value.getB());
//    Matrix.struct.pack(bb, value.getC());
//    Matrix.struct.pack(bb, value.getD());
//  }
//}
