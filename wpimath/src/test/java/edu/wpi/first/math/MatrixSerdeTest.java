//// Copyright (c) FIRST and other WPILib contributors.
//// Open Source Software; you can modify and/or share it under the terms of
//// the WPILib BSD license file in the root directory of this project.
//
//package edu.wpi.first.math;
//
//import static org.junit.jupiter.api.Assertions.assertArrayEquals;
//import static org.junit.jupiter.api.Assertions.assertEquals;
//
//import edu.wpi.first.math.proto.Wpimath.ProtobufMatrix;
//import edu.wpi.first.util.struct.Struct;
//import java.nio.ByteBuffer;
//import java.nio.ByteOrder;
//import org.junit.jupiter.api.Test;
//import java.util.Arrays;
//
//public class MatrixSerdeTest {
//  private static final Matrix DATA = new Matrix(1.91, 2.29);
//  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};
//
//  @Test
//  void testStructPack() {
//    ByteBuffer buffer = ByteBuffer.allocate(Matrix.struct.getSize());
//    buffer.order(ByteOrder.LITTLE_ENDIAN);
//    Matrix.struct.pack(buffer, DATA);
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
//    Matrix data = Matrix.struct.unpack(buffer);
//    assertEquals(DATA.getNumRows(), data.getNumRows());
//    assertEquals(DATA.getNumCols(), data.getNumCols());
//    assertEquals(DATA.getData(), data.getData());
//  }
//
//  @Test
//  void testProtoPack() {
//    ProtobufMatrix proto = Matrix.proto.createMessage();
//    Matrix.proto.pack(proto, DATA);
//
//    assertEquals(DATA.getNumRows(), proto.getNumRows());
//    assertEquals(DATA.getNumCols(), proto.getNumCols());
//    assertEquals(DATA.getData(), proto.getData());
//  }
//
//  @Test
//  void testProtoUnpack() {
//    ProtobufMatrix proto = Matrix.proto.createMessage();
//    proto.setNumRows(DATA.getNumRows());
//    proto.setNumCols(DATA.getNumCols());
//    proto.setData(DATA.getData());
//
//    Matrix data = Matrix.proto.unpack(proto);
//    assertEquals(DATA.getNumRows(), data.getNumRows());
//    assertEquals(DATA.getNumCols(), data.getNumCols());
//    assertEquals(DATA.getData(), data.getData());
//  }
//}
