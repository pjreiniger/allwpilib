// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Rotation2d.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Rotation2d>;
using ProtoType = wpi::Protobuf<frc::Rotation2d>;

char kExpectedStructBufferChar[] = {-123, -21, 81, -72, 30, -123, 65, 64};

uint8_t* kExpectedStructBuffer =
    reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

constexpr Rotation2d kExpectedData{35.04_rad};
}  // namespace

TEST(Rotation2dTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(Rotation2dTest, StructUnpack) {
  Rotation2d unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(
          kExpectedStructBuffer, StructType::kSize));

  EXPECT_EQ(kExpectedData.Radians(), unpacked_data.Radians());
}

TEST(Rotation2dTest, ProtobufPack) {
  wpi::proto::ProtobufRotation2d proto;
  ProtoType::Pack(&proto, kExpectedData);

  EXPECT_EQ(kExpectedData.Radians().value(), proto.radians());
}

TEST(Rotation2dTest, ProtobufUnpack) {
  wpi::proto::ProtobufRotation2d proto;
  proto.set_radians(kExpectedData.Radians().value());

  Rotation2d unpacked_data = ProtoType::Unpack(proto);
  EXPECT_EQ(kExpectedData.Radians(), unpacked_data.Radians());
}
