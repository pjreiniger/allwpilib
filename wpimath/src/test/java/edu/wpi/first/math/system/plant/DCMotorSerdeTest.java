//// Copyright (c) FIRST and other WPILib contributors.
//// Open Source Software; you can modify and/or share it under the terms of
//// the WPILib BSD license file in the root directory of this project.
//
//package edu.wpi.first.math.system.plant;
//
//import static org.junit.jupiter.api.Assertions.assertArrayEquals;
//import static org.junit.jupiter.api.Assertions.assertEquals;
//
//import edu.wpi.first.math.proto.Plant.ProtobufDCMotor;
//import edu.wpi.first.util.struct.Struct;
//import java.nio.ByteBuffer;
//import java.nio.ByteOrder;
//import org.junit.jupiter.api.Test;
//import java.util.Arrays;
//
//public class DCMotorSerdeTest {
//  private static final DCMotor DATA = new DCMotor(1.91, 2.29);
//  private static final byte[] STRUCT_BUFFER = new byte[]{63, -2, -113, 92, 40, -11, -62, -113, 64, 2, 81, -21, -123, 30, -72, 82};
//
//  @Test
//  void testStructPack() {
//    ByteBuffer buffer = ByteBuffer.allocate(DCMotor.struct.getSize());
//    buffer.order(ByteOrder.LITTLE_ENDIAN);
//    DCMotor.struct.pack(buffer, DATA);
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
//    DCMotor data = DCMotor.struct.unpack(buffer);
//    assertEquals(DATA.getNominalVoltage(), data.getNominalVoltage());
//    assertEquals(DATA.getStallTorque(), data.getStallTorque());
//    assertEquals(DATA.getStallCurrent(), data.getStallCurrent());
//    assertEquals(DATA.getFreeCurrent(), data.getFreeCurrent());
//    assertEquals(DATA.getFreeSpeed(), data.getFreeSpeed());
//    assertEquals(DATA.getR(), data.getR());
//    assertEquals(DATA.getKv(), data.getKv());
//    assertEquals(DATA.getKt(), data.getKt());
//  }
//
//  @Test
//  void testProtoPack() {
//    ProtobufDCMotor proto = DCMotor.proto.createMessage();
//    DCMotor.proto.pack(proto, DATA);
//
//    assertEquals(DATA.getNominalVoltage(), proto.getNominalVoltage());
//    assertEquals(DATA.getStallTorque(), proto.getStallTorque());
//    assertEquals(DATA.getStallCurrent(), proto.getStallCurrent());
//    assertEquals(DATA.getFreeCurrent(), proto.getFreeCurrent());
//    assertEquals(DATA.getFreeSpeed(), proto.getFreeSpeed());
//    assertEquals(DATA.getR(), proto.getR());
//    assertEquals(DATA.getKv(), proto.getKv());
//    assertEquals(DATA.getKt(), proto.getKt());
//  }
//
//  @Test
//  void testProtoUnpack() {
//    ProtobufDCMotor proto = DCMotor.proto.createMessage();
//    proto.setNominalVoltage(DATA.getNominalVoltage());
//    proto.setStallTorque(DATA.getStallTorque());
//    proto.setStallCurrent(DATA.getStallCurrent());
//    proto.setFreeCurrent(DATA.getFreeCurrent());
//    proto.setFreeSpeed(DATA.getFreeSpeed());
//    proto.setR(DATA.getR());
//    proto.setKv(DATA.getKv());
//    proto.setKt(DATA.getKt());
//
//    DCMotor data = DCMotor.proto.unpack(proto);
//    assertEquals(DATA.getNominalVoltage(), data.getNominalVoltage());
//    assertEquals(DATA.getStallTorque(), data.getStallTorque());
//    assertEquals(DATA.getStallCurrent(), data.getStallCurrent());
//    assertEquals(DATA.getFreeCurrent(), data.getFreeCurrent());
//    assertEquals(DATA.getFreeSpeed(), data.getFreeSpeed());
//    assertEquals(DATA.getR(), data.getR());
//    assertEquals(DATA.getKv(), data.getKv());
//    assertEquals(DATA.getKt(), data.getKt());
//  }
//}
