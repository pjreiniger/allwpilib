// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/MecanumDriveMotorVoltages.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::MecanumDriveMotorVoltages>;
using ProtoType = wpi::Protobuf<frc::MecanumDriveMotorVoltages>;

constexpr std::array<uint8_t, StructType::kSize> create_test_buffer() {
  std::array<uint8_t, StructType::kSize> output;
  int buffer[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
  for (size_t idx = 0; idx < StructType::kSize; ++idx) {
    output[idx] = static_cast<uint8_t>(buffer[idx]);
  }
  return output;
}

std::array<uint8_t, StructType::kSize> kExpectedStructBuffer =
    create_test_buffer();

constexpr MecanumDriveMotorVoltages kExpectedData{};
}  // namespace

TEST(MecanumDriveMotorVoltagesTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(MecanumDriveMotorVoltagesTest, StructUnpack) {
  MecanumDriveMotorVoltages unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.FrontLeftVolts(), unpacked_data.FrontLeftVolts());
    EXPECT_EQ(kExpectedData.FrontRightVolts(), unpacked_data.FrontRightVolts());
    EXPECT_EQ(kExpectedData.RearLeftVolts(), unpacked_data.RearLeftVolts());
    EXPECT_EQ(kExpectedData.RearRightVolts(), unpacked_data.RearRightVolts());
}


TEST(MecanumDriveMotorVoltagesTest, ProtobufPack) {
  wpi::proto::ProtobufMecanumDriveMotorVoltages proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.FrontLeftVolts(), proto.front_left_volts());
    EXPECT_EQ(kExpectedData.FrontRightVolts(), proto.front_right_volts());
    EXPECT_EQ(kExpectedData.RearLeftVolts(), proto.rear_left_volts());
    EXPECT_EQ(kExpectedData.RearRightVolts(), proto.rear_right_volts());
}



TEST(MecanumDriveMotorVoltagesTest, ProtobufUnpack) {
  wpi::proto::ProtobufMecanumDriveMotorVoltages proto;
  proto.set_front_left_volts(kExpectedData.FrontLeftVolts());
  proto.set_front_right_volts(kExpectedData.FrontRightVolts());
  proto.set_rear_left_volts(kExpectedData.RearLeftVolts());
  proto.set_rear_right_volts(kExpectedData.RearRightVolts());

  MecanumDriveMotorVoltages unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.FrontLeftVolts(), unpacked_data.FrontLeftVolts());
    EXPECT_EQ(kExpectedData.FrontRightVolts(), unpacked_data.FrontRightVolts());
    EXPECT_EQ(kExpectedData.RearLeftVolts(), unpacked_data.RearLeftVolts());
    EXPECT_EQ(kExpectedData.RearRightVolts(), unpacked_data.RearRightVolts());
}