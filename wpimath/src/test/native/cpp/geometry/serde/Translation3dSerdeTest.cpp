// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Translation3d.h"
#include "geometry3d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Translation3d>;
using ProtoType = wpi::Protobuf<frc::Translation3d>;

char kExpectedStructBufferChar[] = {
        -102, -103, -103, -103, -103, 25, 51, 64, 102, 102, 102, 102, 102, -26, 54, 64, -43, 120,
        -23, 38, 49, 8, 12, 64
};

uint8_t* kExpectedStructBuffer = reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

constexpr Translation3d kExpectedData{19.1_m, 22.9_m, 3.504_m};
}  // namespace

TEST(Translation3dTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(Translation3dTest, StructUnpack) {
  Translation3d unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(kExpectedStructBuffer, StructType::kSize));

    EXPECT_EQ(kExpectedData.X().value(), unpacked_data.X().value());
    EXPECT_EQ(kExpectedData.Y().value(), unpacked_data.Y().value());
    EXPECT_EQ(kExpectedData.Z().value(), unpacked_data.Z().value());
}


TEST(Translation3dTest, ProtobufPack) {
  wpi::proto::ProtobufTranslation3d proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.X().value(), proto.x_meters());
    EXPECT_EQ(kExpectedData.Y().value(), proto.y_meters());
    EXPECT_EQ(kExpectedData.Z().value(), proto.z_meters());
}



TEST(Translation3dTest, ProtobufUnpack) {
  wpi::proto::ProtobufTranslation3d proto;
  proto.set_x_meters(kExpectedData.X().value());
  proto.set_y_meters(kExpectedData.Y().value());
  proto.set_z_meters(kExpectedData.Z().value());

  Translation3d unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.X().value(), unpacked_data.X().value());
    EXPECT_EQ(kExpectedData.Y().value(), unpacked_data.Y().value());
    EXPECT_EQ(kExpectedData.Z().value(), unpacked_data.Z().value());
}