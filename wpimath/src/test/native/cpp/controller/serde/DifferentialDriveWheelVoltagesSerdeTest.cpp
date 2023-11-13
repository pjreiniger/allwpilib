// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/DifferentialDriveWheelVoltages.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::DifferentialDriveWheelVoltages>;
using ProtoType = wpi::Protobuf<frc::DifferentialDriveWheelVoltages>;

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

constexpr DifferentialDriveWheelVoltages kExpectedData{};
}  // namespace

TEST(DifferentialDriveWheelVoltagesTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(DifferentialDriveWheelVoltagesTest, StructUnpack) {
  DifferentialDriveWheelVoltages unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.Left(), unpacked_data.Left());
    EXPECT_EQ(kExpectedData.Right(), unpacked_data.Right());
}


TEST(DifferentialDriveWheelVoltagesTest, ProtobufPack) {
  wpi::proto::ProtobufDifferentialDriveWheelVoltages proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.Left(), proto.left());
    EXPECT_EQ(kExpectedData.Right(), proto.right());
}



TEST(DifferentialDriveWheelVoltagesTest, ProtobufUnpack) {
  wpi::proto::ProtobufDifferentialDriveWheelVoltages proto;
  proto.set_left(kExpectedData.Left());
  proto.set_right(kExpectedData.Right());

  DifferentialDriveWheelVoltages unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.Left(), unpacked_data.Left());
    EXPECT_EQ(kExpectedData.Right(), unpacked_data.Right());
}