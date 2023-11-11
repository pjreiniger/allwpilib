// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Pose2d.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Pose2d>;
using ProtoType = wpi::Protobuf<frc::Pose2d>;

char kExpectedStructBufferChar[] = {-113, -62, -11, 40,   92,  -113, -2,  63,
                                    82,   -72, 30,  -123, -21, 81,   2,   64,
                                    -41,  -77, 36,  88,   -19, -111, -29, 63};

uint8_t* kExpectedStructBuffer =
    reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

constexpr Pose2d kExpectedData{Translation2d(1.91_m, 2.29_m),
                               Rotation2d(35.04_deg)};
}  // namespace

TEST(Pose2dTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(Pose2dTest, StructUnpack) {
  Pose2d unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(
          kExpectedStructBuffer, StructType::kSize));

  EXPECT_EQ(kExpectedData.Translation(), unpacked_data.Translation());
  EXPECT_EQ(kExpectedData.Rotation(), unpacked_data.Rotation());
}

TEST(Pose2dTest, ProtobufPack) {
  wpi::proto::ProtobufPose2d proto;
  ProtoType::Pack(&proto, kExpectedData);

  EXPECT_EQ(kExpectedData.Translation().X().value(),
            proto.translation().x_meters());
  EXPECT_EQ(kExpectedData.Translation().Y().value(),
            proto.translation().y_meters());
  EXPECT_EQ(kExpectedData.Rotation().Radians().value(),
            proto.rotation().radians());
}

TEST(Pose2dTest, ProtobufUnpack) {
  wpi::proto::ProtobufPose2d proto;
  proto.mutable_translation()->set_x_meters(
      kExpectedData.Translation().X().value());
  proto.mutable_translation()->set_y_meters(
      kExpectedData.Translation().Y().value());
  proto.mutable_rotation()->set_radians(
      kExpectedData.Rotation().Radians().value());

  Pose2d unpacked_data = ProtoType::Unpack(proto);
  EXPECT_EQ(kExpectedData.Translation(), unpacked_data.Translation());
  EXPECT_EQ(kExpectedData.Rotation(), unpacked_data.Rotation());
}
