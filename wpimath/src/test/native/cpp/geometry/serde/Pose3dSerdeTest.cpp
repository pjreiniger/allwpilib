// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Pose3d.h"
#include "geometry3d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Pose3d>;
using ProtoType = wpi::Protobuf<frc::Pose3d>;

char kExpectedStructBufferChar[] = {
    -113, -62,  -11, 40,  92,   -113, -2,   63,  82,  -72, 30,  -123, -21, 81,
    2,    64,   102, 102, 102,  102,  102,  102, 49,  64,  84,  -54,  119, -113,
    -40,  -106, -38, -65, -101, 58,   -111, -47, -66, -90, -20, 63,   30,  41,
    -55,  -118, 53,  68,  -61,  63,   -76,  -25, -32, -83, -71, 105,  -84, 63};

uint8_t* kExpectedStructBuffer =
    reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

const Pose3d kExpectedData{1.91_m, 2.29_m, 17.4_m,
                           Rotation3d(35.4_rad, 12.34_rad, 56.78_rad)};
}  // namespace

TEST(Pose3dTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(Pose3dTest, StructUnpack) {
  Pose3d unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(
          kExpectedStructBuffer, StructType::kSize));

  EXPECT_EQ(kExpectedData.Translation(), unpacked_data.Translation());
  EXPECT_EQ(kExpectedData.Rotation(), unpacked_data.Rotation());
}

TEST(Pose3dTest, ProtobufPack) {
  wpi::proto::ProtobufPose3d proto;
  ProtoType::Pack(&proto, kExpectedData);

  EXPECT_EQ(kExpectedData.Translation().X().value(),
            proto.translation().x_meters());
  EXPECT_EQ(kExpectedData.Translation().Y().value(),
            proto.translation().y_meters());
  EXPECT_EQ(kExpectedData.Translation().Z().value(),
            proto.translation().z_meters());
  EXPECT_EQ(kExpectedData.Rotation().GetQuaternion().W(),
            proto.rotation().q().w());
  EXPECT_EQ(kExpectedData.Rotation().GetQuaternion().X(),
            proto.rotation().q().x());
  EXPECT_EQ(kExpectedData.Rotation().GetQuaternion().Y(),
            proto.rotation().q().y());
  EXPECT_EQ(kExpectedData.Rotation().GetQuaternion().Z(),
            proto.rotation().q().z());
}

TEST(Pose3dTest, ProtobufUnpack) {
  wpi::proto::ProtobufPose3d proto;
  proto.mutable_translation()->set_x_meters(
      kExpectedData.Translation().X().value());
  proto.mutable_translation()->set_y_meters(
      kExpectedData.Translation().Y().value());
  proto.mutable_translation()->set_z_meters(
      kExpectedData.Translation().Z().value());
  proto.mutable_rotation()->mutable_q()->set_w(
      kExpectedData.Rotation().GetQuaternion().W());
  proto.mutable_rotation()->mutable_q()->set_x(
      kExpectedData.Rotation().GetQuaternion().X());
  proto.mutable_rotation()->mutable_q()->set_y(
      kExpectedData.Rotation().GetQuaternion().Y());
  proto.mutable_rotation()->mutable_q()->set_z(
      kExpectedData.Rotation().GetQuaternion().Z());

  Pose3d unpacked_data = ProtoType::Unpack(proto);
  EXPECT_EQ(kExpectedData.Translation(), unpacked_data.Translation());
  EXPECT_EQ(kExpectedData.Rotation(), unpacked_data.Rotation());
}
