// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/DifferentialDriveWheelSpeeds.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::DifferentialDriveWheelSpeeds>;
using ProtoType = wpi::Protobuf<frc::DifferentialDriveWheelSpeeds>;

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

constexpr DifferentialDriveWheelSpeeds kExpectedData{};
}  // namespace

TEST(DifferentialDriveWheelSpeedsTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(DifferentialDriveWheelSpeedsTest, StructUnpack) {
  DifferentialDriveWheelSpeeds unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.LeftMps(), unpacked_data.LeftMps());
    EXPECT_EQ(kExpectedData.RightMps(), unpacked_data.RightMps());
}


TEST(DifferentialDriveWheelSpeedsTest, ProtobufPack) {
  wpi::proto::ProtobufDifferentialDriveWheelSpeeds proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.LeftMps(), proto.left_mps());
    EXPECT_EQ(kExpectedData.RightMps(), proto.right_mps());
}



TEST(DifferentialDriveWheelSpeedsTest, ProtobufUnpack) {
  wpi::proto::ProtobufDifferentialDriveWheelSpeeds proto;
  proto.set_left_mps(kExpectedData.LeftMps());
  proto.set_right_mps(kExpectedData.RightMps());

  DifferentialDriveWheelSpeeds unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.LeftMps(), unpacked_data.LeftMps());
    EXPECT_EQ(kExpectedData.RightMps(), unpacked_data.RightMps());
}