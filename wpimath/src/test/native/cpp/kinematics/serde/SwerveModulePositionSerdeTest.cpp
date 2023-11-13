// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/kinematics/SwerveModulePosition.h"
#include "kinematics.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::SwerveModulePosition>;
using ProtoType = wpi::Protobuf<frc::SwerveModulePosition>;

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

constexpr SwerveModulePosition kExpectedData{};
}  // namespace

TEST(SwerveModulePositionSerdeTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(SwerveModulePositionSerdeTest, StructUnpack) {
  SwerveModulePosition unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.distance.value(), unpacked_data.distance.value());
    // EXPECT_EQ(kExpectedData.angle.value(), unpacked_data.angle.value());
}


TEST(SwerveModulePositionSerdeTest, ProtobufPack) {
  wpi::proto::ProtobufSwerveModulePosition proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.distance.value(), proto.distance_meters());
    // EXPECT_EQ(kExpectedData.angle.value(), proto.angle());
}



TEST(SwerveModulePositionSerdeTest, ProtobufUnpack) {
  wpi::proto::ProtobufSwerveModulePosition proto;
  proto.set_distance_meters(kExpectedData.distance.value());
  // proto.mutable_angle()->(kExpectedData.angle.value());

  SwerveModulePosition unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.distance.value(), unpacked_data.distance.value());
    // EXPECT_EQ(kExpectedData.angle.value(), unpacked_data.angle.value());
}