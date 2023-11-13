// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/MecanumDriveWheelPositions.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::MecanumDriveWheelPositions>;
using ProtoType = wpi::Protobuf<frc::MecanumDriveWheelPositions>;

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

constexpr MecanumDriveWheelPositions kExpectedData{};
}  // namespace

TEST(MecanumDriveWheelPositionsTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(MecanumDriveWheelPositionsTest, StructUnpack) {
  MecanumDriveWheelPositions unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.FrontLeft(), unpacked_data.FrontLeft());
    EXPECT_EQ(kExpectedData.FrontRight(), unpacked_data.FrontRight());
    EXPECT_EQ(kExpectedData.RearLeft(), unpacked_data.RearLeft());
    EXPECT_EQ(kExpectedData.RearRight(), unpacked_data.RearRight());
}


TEST(MecanumDriveWheelPositionsTest, ProtobufPack) {
  wpi::proto::ProtobufMecanumDriveWheelPositions proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.FrontLeft(), proto.front_left_meters());
    EXPECT_EQ(kExpectedData.FrontRight(), proto.front_right_meters());
    EXPECT_EQ(kExpectedData.RearLeft(), proto.rear_left_meters());
    EXPECT_EQ(kExpectedData.RearRight(), proto.rear_right_meters());
}



TEST(MecanumDriveWheelPositionsTest, ProtobufUnpack) {
  wpi::proto::ProtobufMecanumDriveWheelPositions proto;
  proto.set_front_left_meters(kExpectedData.FrontLeft());
  proto.set_front_right_meters(kExpectedData.FrontRight());
  proto.set_rear_left_meters(kExpectedData.RearLeft());
  proto.set_rear_right_meters(kExpectedData.RearRight());

  MecanumDriveWheelPositions unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.FrontLeft(), unpacked_data.FrontLeft());
    EXPECT_EQ(kExpectedData.FrontRight(), unpacked_data.FrontRight());
    EXPECT_EQ(kExpectedData.RearLeft(), unpacked_data.RearLeft());
    EXPECT_EQ(kExpectedData.RearRight(), unpacked_data.RearRight());
}