// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/SwerveModulePosition.h"
#include "geometry2d.pb.h"

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

TEST(SwerveModulePositionTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(SwerveModulePositionTest, StructUnpack) {
  SwerveModulePosition unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.Distance(), unpacked_data.Distance());
    EXPECT_EQ(kExpectedData.Angle(), unpacked_data.Angle());
}


TEST(SwerveModulePositionTest, ProtobufPack) {
  wpi::proto::ProtobufSwerveModulePosition proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.Distance(), proto.distance_meters());
    EXPECT_EQ(kExpectedData.Angle(), proto.angle());
}



TEST(SwerveModulePositionTest, ProtobufUnpack) {
  wpi::proto::ProtobufSwerveModulePosition proto;
  proto.set_distance_meters(kExpectedData.Distance());
  proto.mutable_angle()->(kExpectedData.Angle());

  SwerveModulePosition unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.Distance(), unpacked_data.Distance());
    EXPECT_EQ(kExpectedData.Angle(), unpacked_data.Angle());
}