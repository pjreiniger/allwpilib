// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <gtest/gtest.h>

#include "frc/geometry/SimpleMotorFeedforward.h"
#include "geometry2d.pb.h"

using namespace frc;

namespace {

using StructType = wpi::Struct<frc::SimpleMotorFeedforward>;
using ProtoType = wpi::Protobuf<frc::SimpleMotorFeedforward>;

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

constexpr SimpleMotorFeedforward kExpectedData{};
}  // namespace

TEST(SimpleMotorFeedforwardTest, StructPack) {
  uint8_t buffer[StructType::kSize];
  StructType::Pack(buffer, kExpectedData);

  for (size_t i = 0; i < StructType::kSize; ++i) {
    EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
  }
}

TEST(SimpleMotorFeedforwardTest, StructUnpack) {
  SimpleMotorFeedforward unpacked_data = StructType::Unpack(kExpectedStructBuffer);

    EXPECT_EQ(kExpectedData.Ks(), unpacked_data.Ks());
    EXPECT_EQ(kExpectedData.Kv(), unpacked_data.Kv());
    EXPECT_EQ(kExpectedData.Ka(), unpacked_data.Ka());
}


TEST(SimpleMotorFeedforwardTest, ProtobufPack) {
  wpi::proto::ProtobufSimpleMotorFeedforward proto;
  ProtoType::Pack(&proto, kExpectedData);

    EXPECT_EQ(kExpectedData.Ks(), proto.ks());
    EXPECT_EQ(kExpectedData.Kv(), proto.kv());
    EXPECT_EQ(kExpectedData.Ka(), proto.ka());
}



TEST(SimpleMotorFeedforwardTest, ProtobufUnpack) {
  wpi::proto::ProtobufSimpleMotorFeedforward proto;
  proto.set_ks(kExpectedData.Ks());
  proto.set_kv(kExpectedData.Kv());
  proto.set_ka(kExpectedData.Ka());

  SimpleMotorFeedforward unpacked_data = ProtoType::Unpack(proto);
    EXPECT_EQ(kExpectedData.Ks(), unpacked_data.Ks());
    EXPECT_EQ(kExpectedData.Kv(), unpacked_data.Kv());
    EXPECT_EQ(kExpectedData.Ka(), unpacked_data.Ka());
}