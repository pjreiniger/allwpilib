// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/ArmFeedforward.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::ArmFeedforward>;
using ProtoType = wpi::Protobuf<frc::ArmFeedforward>;

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

constexpr ArmFeedforward kExpectedData{};
}  // namespace

TEST(ArmFeedforwardTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(ArmFeedforwardTest, StructUnpack) {
  ArmFeedforward unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.Ks(), unpacked_data.Ks());
    EXPECT_EQ(kExpectedData.Kg(), unpacked_data.Kg());
    EXPECT_EQ(kExpectedData.Kv(), unpacked_data.Kv());
    EXPECT_EQ(kExpectedData.Ka(), unpacked_data.Ka());
}


TEST(ArmFeedforwardTest, ProtobufPack) {
  wpi::proto::ProtobufArmFeedforward proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.Ks(), proto.ks());
    EXPECT_EQ(kExpectedData.Kg(), proto.kg());
    EXPECT_EQ(kExpectedData.Kv(), proto.kv());
    EXPECT_EQ(kExpectedData.Ka(), proto.ka());
}



TEST(ArmFeedforwardTest, ProtobufUnpack) {
  wpi::proto::ProtobufArmFeedforward proto;
  proto.set_ks(kExpectedData.Ks());
  proto.set_kg(kExpectedData.Kg());
  proto.set_kv(kExpectedData.Kv());
  proto.set_ka(kExpectedData.Ka());

  ArmFeedforward unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.Ks(), unpacked_data.Ks());
    EXPECT_EQ(kExpectedData.Kg(), unpacked_data.Kg());
    EXPECT_EQ(kExpectedData.Kv(), unpacked_data.Kv());
    EXPECT_EQ(kExpectedData.Ka(), unpacked_data.Ka());
}