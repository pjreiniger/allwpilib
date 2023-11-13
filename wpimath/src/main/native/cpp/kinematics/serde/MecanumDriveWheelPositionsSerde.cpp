// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/kinematics/MecanumDriveWheelPositions.h"
#include "kinematics.pb.h"

using StructType = wpi::Struct<frc::MecanumDriveWheelPositions>;

// // frc::MecanumDriveWheelPositions StructType::Unpack(
// //     std::span<const uint8_t, StructType::kSize> data) {
// //   return {
// //     units::meter_t{wpi::UnpackStruct<double, 0>(data)},
// //     units::meter_t{wpi::UnpackStruct<double, 0>(data)},
// //     units::meter_t{wpi::UnpackStruct<double, 0>(data)},
// //     units::meter_t{wpi::UnpackStruct<double, 0>(data)},
// //   };
// // }

// void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
//                       const frc::MecanumDriveWheelPositions& value) {
//     // wpi::PackStruct<0>(data, value.FrontLeft());
//     // wpi::PackStruct<0>(data, value.FrontRight());
//     // wpi::PackStruct<0>(data, value.RearLeft());
//     // wpi::PackStruct<0>(data, value.RearRight());
// }

// google::protobuf::Message* wpi::Protobuf<frc::MecanumDriveWheelPositions>::New(
//     google::protobuf::Arena* arena) {
//   return google::protobuf::Arena::CreateMessage<
//       wpi::proto::ProtobufMecanumDriveWheelPositions>(arena);
// }

// frc::MecanumDriveWheelPositions wpi::Protobuf<frc::MecanumDriveWheelPositions>::Unpack(
//     const google::protobuf::Message& msg) {
//   auto m = static_cast<const wpi::proto::ProtobufMecanumDriveWheelPositions*>(&msg);
//   return frc::MecanumDriveWheelPositions{
//     // units::meter_t{m->front_left_meters()},
//     // units::meter_t{m->front_right_meters()},
//     // units::meter_t{m->rear_left_meters()},
//     // units::meter_t{m->rear_right_meters()},
//   };
// }

// void wpi::Protobuf<frc::MecanumDriveWheelPositions>::Pack(google::protobuf::Message* msg,
//                                              const frc::MecanumDriveWheelPositions& value) {
// //   auto m = static_cast<wpi::proto::ProtobufMecanumDriveWheelPositions*>(msg);
// //     m->set_front_left_meters(value.FrontLeft());
// //     m->set_front_right_meters(value.FrontRight());
// //     m->set_rear_left_meters(value.RearLeft());
// //     m->set_rear_right_meters(value.RearRight());
// }
