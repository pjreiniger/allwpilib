// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/kinematics/MecanumDriveWheelSpeeds.h"
#include "kinematics.pb.h"

using StructType = wpi::Struct<frc::MecanumDriveWheelSpeeds>;

// frc::MecanumDriveWheelSpeeds StructType::Unpack(
//     std::span<const uint8_t, StructType::kSize> data) {
//   return {
//     wpi::UnpackStruct<double, 0>(data),
//     wpi::UnpackStruct<double, 0>(data),
//     wpi::UnpackStruct<double, 0>(data),
//     wpi::UnpackStruct<double, 0>(data),
//   };
// }

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::MecanumDriveWheelSpeeds& value) {
    // wpi::PackStruct<0>(data, value.FrontLeftMps());
    // wpi::PackStruct<0>(data, value.FrontRightMps());
    // wpi::PackStruct<0>(data, value.RearLeftMps());
    // wpi::PackStruct<0>(data, value.RearRightMps());
}

google::protobuf::Message* wpi::Protobuf<frc::MecanumDriveWheelSpeeds>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufMecanumDriveWheelSpeeds>(arena);
}

// frc::MecanumDriveWheelSpeeds wpi::Protobuf<frc::MecanumDriveWheelSpeeds>::Unpack(
//     const google::protobuf::Message& msg) {
//   auto m = static_cast<const wpi::proto::ProtobufMecanumDriveWheelSpeeds*>(&msg);
//   return frc::MecanumDriveWheelSpeeds{
//     m->front_left_mps(),
//     m->front_right_mps(),
//     m->rear_left_mps(),
//     m->rear_right_mps(),
//   };
// }

void wpi::Protobuf<frc::MecanumDriveWheelSpeeds>::Pack(google::protobuf::Message* msg,
                                             const frc::MecanumDriveWheelSpeeds& value) {
//   auto m = static_cast<wpi::proto::ProtobufMecanumDriveWheelSpeeds*>(msg);
//     m->set_front_left_mps(value.FrontLeftMps());
//     m->set_front_right_mps(value.FrontRightMps());
//     m->set_rear_left_mps(value.RearLeftMps());
//     m->set_rear_right_mps(value.RearRightMps());
}
