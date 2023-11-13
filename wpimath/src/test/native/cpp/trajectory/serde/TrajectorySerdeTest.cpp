// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

// #include <gtest/gtest.h>

// #include "frc/trajectory/Trajectory.h"
// #include "trajectory.pb.h"

// using namespace frc;

// namespace {

// using StructType = wpi::Struct<frc::Trajectory>;
// using ProtoType = wpi::Protobuf<frc::Trajectory>;

// constexpr std::array<uint8_t, StructType::kSize> create_test_buffer() {
//   std::array<uint8_t, StructType::kSize> output;
//   int buffer[] = {
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
// };
//   for (size_t idx = 0; idx < StructType::kSize; ++idx) {
//     output[idx] = static_cast<uint8_t>(buffer[idx]);
//   }
//   return output;
// }

// std::array<uint8_t, StructType::kSize> kExpectedStructBuffer =
//     create_test_buffer();

// constexpr Trajectory kExpectedData{};
// }  // namespace

// TEST(TrajectorySerdeTest, StructPack) {
//   uint8_t buffer[StructType::kSize];
//   StructType::Pack(buffer, kExpectedData);

//   for (size_t i = 0; i < StructType::kSize; ++i) {
//     EXPECT_EQ(kExpectedStructBuffer[i], buffer[i]) << " on byte " << i;
//   }
// }

// TEST(TrajectorySerdeTest, StructUnpack) {
//   Trajectory unpacked_data = StructType::Unpack(kExpectedStructBuffer);

//     EXPECT_EQ(kExpectedData.states.value(), unpacked_data.states.value());
// }

// TEST(TrajectorySerdeTest, ProtobufPack) {
//   wpi::proto::ProtobufTrajectory proto;
//   ProtoType::Pack(&proto, kExpectedData);

//     EXPECT_EQ(kExpectedData.states.value(), proto.states());
// }

// TEST(TrajectorySerdeTest, ProtobufUnpack) {
//   wpi::proto::ProtobufTrajectory proto;
//   proto.mutable_states()->(kExpectedData.states.value());

//   Trajectory unpacked_data = ProtoType::Unpack(proto);
//     EXPECT_EQ(kExpectedData.states.value(), unpacked_data.states.value());
// }
