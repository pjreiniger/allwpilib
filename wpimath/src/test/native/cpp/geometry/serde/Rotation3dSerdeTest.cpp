// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/Rotation3d.h"
#include "geometry3d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::Rotation3d>;
using ProtoType = wpi::Protobuf<frc::Rotation3d>;

char kExpectedStructBufferChar[] = {10, -88, 124, 104, -38,  70,  -27, -65,
                                    95, 75,  113, 113, 97,   -20, -30, 63,
                                    11, -40, 66,  111, -115, -46, -55, -65,
                                    44, -40, 31,  20,  -91,  49,  -38, -65};

uint8_t* kExpectedStructBuffer =
    reinterpret_cast<uint8_t*>(kExpectedStructBufferChar);

const Rotation3d kExpectedData{1.91_rad, 2.29_rad, 35.04_rad};
}  // namespace

TEST(Rotation3dTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(Rotation3dTest, StructUnpack) {
  Rotation3d unpacked_data =
      StructType::Unpack(std::span<uint8_t, StructType::kSize>(
          kExpectedStructBuffer, StructType::kSize));

  EXPECT_EQ(kExpectedData.GetQuaternion(), unpacked_data.GetQuaternion());
}

TEST(Rotation3dTest, ProtobufPack) {
  wpi::proto::ProtobufRotation3d proto;
  ProtoType::Pack(&proto, kExpectedData);

  EXPECT_EQ(kExpectedData.GetQuaternion().W(), proto.q().w());
  EXPECT_EQ(kExpectedData.GetQuaternion().X(), proto.q().x());
  EXPECT_EQ(kExpectedData.GetQuaternion().Y(), proto.q().y());
  EXPECT_EQ(kExpectedData.GetQuaternion().Z(), proto.q().z());
}

TEST(Rotation3dTest, ProtobufUnpack) {
  wpi::proto::ProtobufRotation3d proto;
  proto.mutable_q()->set_w(kExpectedData.GetQuaternion().W());
  proto.mutable_q()->set_x(kExpectedData.GetQuaternion().X());
  proto.mutable_q()->set_y(kExpectedData.GetQuaternion().Y());
  proto.mutable_q()->set_z(kExpectedData.GetQuaternion().Z());

  Rotation3d unpacked_data = ProtoType::Unpack(proto);
  EXPECT_EQ(kExpectedData.GetQuaternion(), unpacked_data.GetQuaternion());
}
