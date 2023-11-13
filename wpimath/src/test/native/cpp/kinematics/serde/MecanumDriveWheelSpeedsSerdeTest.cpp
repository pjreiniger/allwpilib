// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/MecanumDriveWheelSpeeds.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::MecanumDriveWheelSpeeds>;
using ProtoType = wpi::Protobuf<frc::MecanumDriveWheelSpeeds>;

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

constexpr MecanumDriveWheelSpeeds kExpectedData{};
}  // namespace

TEST(MecanumDriveWheelSpeedsTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(MecanumDriveWheelSpeedsTest, StructUnpack) {
  MecanumDriveWheelSpeeds unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.FrontLeftMps(), unpacked_data.FrontLeftMps());
    EXPECT_EQ(kExpectedData.FrontRightMps(), unpacked_data.FrontRightMps());
    EXPECT_EQ(kExpectedData.RearLeftMps(), unpacked_data.RearLeftMps());
    EXPECT_EQ(kExpectedData.RearRightMps(), unpacked_data.RearRightMps());
}


TEST(MecanumDriveWheelSpeedsTest, ProtobufPack) {
  wpi::proto::ProtobufMecanumDriveWheelSpeeds proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.FrontLeftMps(), proto.front_left_mps());
    EXPECT_EQ(kExpectedData.FrontRightMps(), proto.front_right_mps());
    EXPECT_EQ(kExpectedData.RearLeftMps(), proto.rear_left_mps());
    EXPECT_EQ(kExpectedData.RearRightMps(), proto.rear_right_mps());
}



TEST(MecanumDriveWheelSpeedsTest, ProtobufUnpack) {
  wpi::proto::ProtobufMecanumDriveWheelSpeeds proto;
  proto.set_front_left_mps(kExpectedData.FrontLeftMps());
  proto.set_front_right_mps(kExpectedData.FrontRightMps());
  proto.set_rear_left_mps(kExpectedData.RearLeftMps());
  proto.set_rear_right_mps(kExpectedData.RearRightMps());

  MecanumDriveWheelSpeeds unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.FrontLeftMps(), unpacked_data.FrontLeftMps());
    EXPECT_EQ(kExpectedData.FrontRightMps(), unpacked_data.FrontRightMps());
    EXPECT_EQ(kExpectedData.RearLeftMps(), unpacked_data.RearLeftMps());
    EXPECT_EQ(kExpectedData.RearRightMps(), unpacked_data.RearRightMps());
}