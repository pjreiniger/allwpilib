// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Translation2d.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Translation2d>;
using ProtoType = wpi::Protobuf<frc::Translation2d>;

char kExpectedStructBufferChar[] = {-113, -62, -11, 40,   92,  -113, -2, 63,
                                    82,   -72, 30,  -123, -21, 81,   2,  64};

uint8_t* kExpectedStructBuffer =
    reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

constexpr Translation2d kExpectedData{1.91_m, 2.29_m};
}  // namespace

TEST(Translation2dTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(Translation2dTest, StructUnpack) {
  Translation2d unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(
          kExpectedStructBuffer, StructType::kSize));

  EXPECT_EQ(kExpectedData.X().value(), unpacked_data.X().value());
  EXPECT_EQ(kExpectedData.Y().value(), unpacked_data.Y().value());
}

TEST(Translation2dTest, ProtobufPack) {
  wpi::proto::ProtobufTranslation2d proto;
  ProtoType::Pack(&proto, kExpectedData);

  EXPECT_EQ(kExpectedData.X().value(), proto.x_meters());
  EXPECT_EQ(kExpectedData.Y().value(), proto.y_meters());
}

TEST(Translation2dTest, ProtobufUnpack) {
  wpi::proto::ProtobufTranslation2d proto;
  proto.set_x_meters(kExpectedData.X().value());
  proto.set_y_meters(kExpectedData.Y().value());

  Translation2d unpacked_data = ProtoType::Unpack(proto);
  EXPECT_EQ(kExpectedData.X().value(), unpacked_data.X().value());
  EXPECT_EQ(kExpectedData.Y().value(), unpacked_data.Y().value());
}
