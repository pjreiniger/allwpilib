// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/SwerveDriveKinematics.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::SwerveDriveKinematics>;
using ProtoType = wpi::Protobuf<frc::SwerveDriveKinematics>;

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

constexpr SwerveDriveKinematics kExpectedData{};
}  // namespace

TEST(SwerveDriveKinematicsTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(SwerveDriveKinematicsTest, StructUnpack) {
  SwerveDriveKinematics unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.Modules(), unpacked_data.Modules());
}


TEST(SwerveDriveKinematicsTest, ProtobufPack) {
  wpi::proto::ProtobufSwerveDriveKinematics proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.Modules(), proto.modules());
}



TEST(SwerveDriveKinematicsTest, ProtobufUnpack) {
  wpi::proto::ProtobufSwerveDriveKinematics proto;
  proto.mutable_modules()->(kExpectedData.Modules());

  SwerveDriveKinematics unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.Modules(), unpacked_data.Modules());
}