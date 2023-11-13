// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/ChassisSpeeds.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::ChassisSpeeds>;
using ProtoType = wpi::Protobuf<frc::ChassisSpeeds>;

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

constexpr ChassisSpeeds kExpectedData{};
}  // namespace

TEST(ChassisSpeedsTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(ChassisSpeedsTest, StructUnpack) {
  ChassisSpeeds unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.VxMps(), unpacked_data.VxMps());
    EXPECT_EQ(kExpectedData.VyMps(), unpacked_data.VyMps());
    EXPECT_EQ(kExpectedData.OmegaRps(), unpacked_data.OmegaRps());
}


TEST(ChassisSpeedsTest, ProtobufPack) {
  wpi::proto::ProtobufChassisSpeeds proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.VxMps(), proto.vx_mps());
    EXPECT_EQ(kExpectedData.VyMps(), proto.vy_mps());
    EXPECT_EQ(kExpectedData.OmegaRps(), proto.omega_rps());
}



TEST(ChassisSpeedsTest, ProtobufUnpack) {
  wpi::proto::ProtobufChassisSpeeds proto;
  proto.set_vx_mps(kExpectedData.VxMps());
  proto.set_vy_mps(kExpectedData.VyMps());
  proto.set_omega_rps(kExpectedData.OmegaRps());

  ChassisSpeeds unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.VxMps(), unpacked_data.VxMps());
    EXPECT_EQ(kExpectedData.VyMps(), unpacked_data.VyMps());
    EXPECT_EQ(kExpectedData.OmegaRps(), unpacked_data.OmegaRps());
}