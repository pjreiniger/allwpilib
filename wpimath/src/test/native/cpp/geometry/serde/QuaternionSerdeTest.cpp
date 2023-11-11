// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Quaternion.h"
#include "geometry3d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Quaternion>;
using ProtoType = wpi::Protobuf<frc::Quaternion>;

char kExpectedStructBufferChar[] = {
        -102, -103, -103, -103, -103, -103, -15, 63, -102, -103, -103, -103, -103, -103, 1, 64, 102,
        102, 102, 102, 102, 102, 10, 64, -102, -103, -103, -103, -103, -103, 17, 64
};

uint8_t* kExpectedStructBuffer = reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

const Quaternion kExpectedData{1.1, 2.2, 3.3, 4.4};
}  // namespace

TEST(QuaternionTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(QuaternionTest, StructUnpack) {
  Quaternion unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(kExpectedStructBuffer, StructType::kSize));

    EXPECT_EQ(kExpectedData.W(), unpacked_data.W());
    EXPECT_EQ(kExpectedData.X(), unpacked_data.X());
    EXPECT_EQ(kExpectedData.Y(), unpacked_data.Y());
    EXPECT_EQ(kExpectedData.Z(), unpacked_data.Z());
}


TEST(QuaternionTest, ProtobufPack) {
  wpi::proto::ProtobufQuaternion proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.W(), proto.w());
    EXPECT_EQ(kExpectedData.X(), proto.x());
    EXPECT_EQ(kExpectedData.Y(), proto.y());
    EXPECT_EQ(kExpectedData.Z(), proto.z());
}



TEST(QuaternionTest, ProtobufUnpack) {
  wpi::proto::ProtobufQuaternion proto;
  proto.set_w(kExpectedData.W());
  proto.set_x(kExpectedData.X());
  proto.set_y(kExpectedData.Y());
  proto.set_z(kExpectedData.Z());

  Quaternion unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.W(), unpacked_data.W());
    EXPECT_EQ(kExpectedData.X(), unpacked_data.X());
    EXPECT_EQ(kExpectedData.Y(), unpacked_data.Y());
    EXPECT_EQ(kExpectedData.Z(), unpacked_data.Z());
}