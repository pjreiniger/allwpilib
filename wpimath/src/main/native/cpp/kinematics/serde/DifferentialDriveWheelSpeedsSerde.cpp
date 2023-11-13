// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/kinematics/DifferentialDriveWheelSpeeds.h"
#include "kinematics.pb.h"

using StructType = wpi::Struct<frc::DifferentialDriveWheelSpeeds>;

frc::DifferentialDriveWheelSpeeds StructType::Unpack(
    std::span<const uint8_t, StructType::kSize> data) {
  return frc::DifferentialDriveWheelSpeeds{
      units::meters_per_second_t{wpi::UnpackStruct<double, 0>(data)},
      units::meters_per_second_t{wpi::UnpackStruct<double, 8>(data)},
  };
}

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::DifferentialDriveWheelSpeeds& value) {
  wpi::PackStruct<0>(data, value.left.value());
  wpi::PackStruct<8>(data, value.right.value());
}

google::protobuf::Message* wpi::Protobuf<
    frc::DifferentialDriveWheelSpeeds>::New(google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufDifferentialDriveWheelSpeeds>(arena);
}

frc::DifferentialDriveWheelSpeeds
wpi::Protobuf<frc::DifferentialDriveWheelSpeeds>::Unpack(
    const google::protobuf::Message& msg) {
  auto m = static_cast<const wpi::proto::ProtobufDifferentialDriveWheelSpeeds*>(
      &msg);
  return frc::DifferentialDriveWheelSpeeds{
      units::meters_per_second_t{m->left_mps()},
      units::meters_per_second_t{m->right_mps()},
  };
}

void wpi::Protobuf<frc::DifferentialDriveWheelSpeeds>::Pack(
    google::protobuf::Message* msg,
    const frc::DifferentialDriveWheelSpeeds& value) {
  auto m = static_cast<wpi::proto::ProtobufDifferentialDriveWheelSpeeds*>(msg);
  m->set_left_mps(value.left.value());
  m->set_right_mps(value.right.value());
}
