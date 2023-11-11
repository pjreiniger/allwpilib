//// Copyright (c) FIRST and other WPILib contributors.
//// Open Source Software; you can modify and/or share it under the terms of
//// the WPILib BSD license file in the root directory of this project.
//
//package edu.wpi.first.math.system;
//
//import static org.junit.jupiter.api.Assertions.assertArrayEquals;
//import static org.junit.jupiter.api.Assertions.assertEquals;
//
//import edu.wpi.first.math.proto.System.ProtobufLinearSystem;
//import edu.wpi.first.util.struct.Struct;
//import java.nio.ByteBuffer;
//import java.nio.ByteOrder;
//import org.junit.jupiter.api.Test;
//import java.util.Arrays;
//
//public class LinearSystemSerdeTest {
//  private static final LinearSystem DATA = new LinearSystem(1.91, 2.29);
//  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};
//
//  @Test
//  void testStructPack() {
//    ByteBuffer buffer = ByteBuffer.allocate(LinearSystem.struct.getSize());
//    buffer.order(ByteOrder.LITTLE_ENDIAN);
//    LinearSystem.struct.pack(buffer, DATA);
//
//    byte[] actual = buffer.array();
//    String newContent = new String(buffer.array());
//    System.out.println(Arrays.toString(actual));
//    System.out.println(newContent);
//    assertArrayEquals(actual, STRUCT_BUFFER);
//  }
//
//  @Test
//  void testStructUnpack() {
//    ByteBuffer buffer = ByteBuffer.wrap(STRUCT_BUFFER);
//    buffer.order(ByteOrder.LITTLE_ENDIAN);
//
//    LinearSystem data = LinearSystem.struct.unpack(buffer);
//    assertEquals(DATA.getNumStates(), data.getNumStates());
//    assertEquals(DATA.getNumInputs(), data.getNumInputs());
//    assertEquals(DATA.getNumOutputs(), data.getNumOutputs());
//    assertEquals(DATA.getA(), data.getA());
//    assertEquals(DATA.getB(), data.getB());
//    assertEquals(DATA.getC(), data.getC());
//    assertEquals(DATA.getD(), data.getD());
//  }
//
//  @Test
//  void testProtoPack() {
//    ProtobufLinearSystem proto = LinearSystem.proto.createMessage();
//    LinearSystem.proto.pack(proto, DATA);
//
//    assertEquals(DATA.getNumStates(), proto.getNumStates());
//    assertEquals(DATA.getNumInputs(), proto.getNumInputs());
//    assertEquals(DATA.getNumOutputs(), proto.getNumOutputs());
//    assertEquals(DATA.getA(), Matrix.proto.unpack(proto.getA()));
//    assertEquals(DATA.getB(), Matrix.proto.unpack(proto.getB()));
//    assertEquals(DATA.getC(), Matrix.proto.unpack(proto.getC()));
//    assertEquals(DATA.getD(), Matrix.proto.unpack(proto.getD()));
//  }
//
//  @Test
//  void testProtoUnpack() {
//    ProtobufLinearSystem proto = LinearSystem.proto.createMessage();
//    proto.setNumStates(DATA.getNumStates());
//    proto.setNumInputs(DATA.getNumInputs());
//    proto.setNumOutputs(DATA.getNumOutputs());
//    Matrix.proto.pack(proto.getMutableA(), DATA.getA());
//    Matrix.proto.pack(proto.getMutableB(), DATA.getB());
//    Matrix.proto.pack(proto.getMutableC(), DATA.getC());
//    Matrix.proto.pack(proto.getMutableD(), DATA.getD());
//
//    LinearSystem data = LinearSystem.proto.unpack(proto);
//    assertEquals(DATA.getNumStates(), data.getNumStates());
//    assertEquals(DATA.getNumInputs(), data.getNumInputs());
//    assertEquals(DATA.getNumOutputs(), data.getNumOutputs());
//    assertEquals(DATA.getA(), data.getA());
//    assertEquals(DATA.getB(), data.getB());
//    assertEquals(DATA.getC(), data.getC());
//    assertEquals(DATA.getD(), data.getD());
//  }
//}
