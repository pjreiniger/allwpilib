// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/trajectory/Trajectory.h"
#include "trajectory.pb.h"

using StructType = wpi::Struct<frc::Trajectory>;

// frc::Trajectory StructType::Unpack(
//     std::span<const uint8_t, StructType::kSize> data) {
//   return {
//     wpi::UnpackStruct<frc::RepeatedCompositeFieldContainer, 0>(data),
//   };
// }

// void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
//                       const frc::Trajectory& value) {
//     wpi::PackStruct<0>(data, value.States());
// }

// void StructType::ForEachNested(
//     std::invocable<std::string_view, std::string_view> auto fn) {
//   wpi::ForEachStructSchema<frc::RepeatedCompositeFieldContainer>(fn);
// }

// google::protobuf::Message* wpi::Protobuf<frc::Trajectory>::New(
//     google::protobuf::Arena* arena) {
//   return google::protobuf::Arena::CreateMessage<
//       wpi::proto::ProtobufTrajectory>(arena);
// }

// frc::Trajectory wpi::Protobuf<frc::Trajectory>::Unpack(
//     const google::protobuf::Message& msg) {
//   auto m = static_cast<const wpi::proto::ProtobufTrajectory*>(&msg);
//   return frc::Trajectory{
//     wpi::UnpackProtobuf<frc::RepeatedCompositeFieldContainer>(m->states()),
//   };
// }

// void wpi::Protobuf<frc::Trajectory>::Pack(google::protobuf::Message* msg,
//                                              const frc::Trajectory& value) {
//   auto m = static_cast<wpi::proto::ProtobufTrajectory*>(msg);
//     wpi::PackProtobuf(m->mutable_states(), value.States());
// }
