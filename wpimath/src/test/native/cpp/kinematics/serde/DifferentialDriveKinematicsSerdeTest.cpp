// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/DifferentialDriveKinematics.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::DifferentialDriveKinematics>;
using ProtoType = wpi::Protobuf<frc::DifferentialDriveKinematics>;

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

constexpr DifferentialDriveKinematics kExpectedData{};
}  // namespace

TEST(DifferentialDriveKinematicsTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(DifferentialDriveKinematicsTest, StructUnpack) {
  DifferentialDriveKinematics unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.TrackWidth(), unpacked_data.TrackWidth());
}


TEST(DifferentialDriveKinematicsTest, ProtobufPack) {
  wpi::proto::ProtobufDifferentialDriveKinematics proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.TrackWidth(), proto.track_width_meters());
}



TEST(DifferentialDriveKinematicsTest, ProtobufUnpack) {
  wpi::proto::ProtobufDifferentialDriveKinematics proto;
  proto.set_track_width_meters(kExpectedData.TrackWidth());

  DifferentialDriveKinematics unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.TrackWidth(), unpacked_data.TrackWidth());
}