// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/kinematics/MecanumDriveWheelSpeeds.h"
#include "kinematics.pb.h"

using StructType = wpi::Struct<frc::MecanumDriveWheelSpeeds>;

frc::MecanumDriveWheelSpeeds StructType::Unpack(
    std::span<const uint8_t, StructType::kSize> data) {
  return frc::MecanumDriveWheelSpeeds{
    units::meters_per_second_t{wpi::UnpackStruct<double, 0>(data)},
    units::meters_per_second_t{wpi::UnpackStruct<double, 8>(data)},
    units::meters_per_second_t{wpi::UnpackStruct<double, 16>(data)},
    units::meters_per_second_t{wpi::UnpackStruct<double, 24>(data)},
  };
}

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::MecanumDriveWheelSpeeds& value) {
    wpi::PackStruct<0>(data, value.frontLeft.value());
    wpi::PackStruct<8>(data, value.frontRight.value());
    wpi::PackStruct<16>(data, value.rearLeft.value());
    wpi::PackStruct<24>(data, value.rearRight.value());
}

google::protobuf::Message* wpi::Protobuf<frc::MecanumDriveWheelSpeeds>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufMecanumDriveWheelSpeeds>(arena);
}

frc::MecanumDriveWheelSpeeds wpi::Protobuf<frc::MecanumDriveWheelSpeeds>::Unpack(
    const google::protobuf::Message& msg) {
  auto m = static_cast<const wpi::proto::ProtobufMecanumDriveWheelSpeeds*>(&msg);
  return frc::MecanumDriveWheelSpeeds{
    units::meters_per_second_t{m->front_left_mps()},
    units::meters_per_second_t{m->front_right_mps()},
    units::meters_per_second_t{m->rear_left_mps()},
    units::meters_per_second_t{m->rear_right_mps()},
  };
}

void wpi::Protobuf<frc::MecanumDriveWheelSpeeds>::Pack(google::protobuf::Message* msg,
                                             const frc::MecanumDriveWheelSpeeds& value) {
  auto m = static_cast<wpi::proto::ProtobufMecanumDriveWheelSpeeds*>(msg);
    m->set_front_left_mps(value.frontLeft.value());
    m->set_front_right_mps(value.frontRight.value());
    m->set_rear_left_mps(value.rearLeft.value());
    m->set_rear_right_mps(value.rearRight.value());
}
