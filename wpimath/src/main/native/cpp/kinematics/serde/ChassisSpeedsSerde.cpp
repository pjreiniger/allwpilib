// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/kinematics/ChassisSpeeds.h"
#include "kinematics.pb.h"

using StructType = wpi::Struct<frc::ChassisSpeeds>;

frc::ChassisSpeeds StructType::Unpack(
    std::span<const uint8_t, StructType::kSize> data) {
  return frc::ChassisSpeeds{
      units::meters_per_second_t{wpi::UnpackStruct<double, 0>(data)},
      units::meters_per_second_t{wpi::UnpackStruct<double, 8>(data)},
      units::radians_per_second_t{wpi::UnpackStruct<double, 16>(data)},
  };
}

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::ChassisSpeeds& value) {
  wpi::PackStruct<0>(data, value.vx.value());
  wpi::PackStruct<8>(data, value.vy.value());
  wpi::PackStruct<16>(data, value.omega.value());
}

google::protobuf::Message* wpi::Protobuf<frc::ChassisSpeeds>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufChassisSpeeds>(arena);
}

frc::ChassisSpeeds wpi::Protobuf<frc::ChassisSpeeds>::Unpack(
    const google::protobuf::Message& msg) {
  auto m = static_cast<const wpi::proto::ProtobufChassisSpeeds*>(&msg);
  return frc::ChassisSpeeds{
      units::meters_per_second_t{m->vx_mps()},
      units::meters_per_second_t{m->vy_mps()},
      units::radians_per_second_t{m->omega_rps()},
  };
}

void wpi::Protobuf<frc::ChassisSpeeds>::Pack(google::protobuf::Message* msg,
                                             const frc::ChassisSpeeds& value) {
  auto m = static_cast<wpi::proto::ProtobufChassisSpeeds*>(msg);
  m->set_vx_mps(value.vx.value());
  m->set_vy_mps(value.vy.value());
  m->set_omega_rps(value.omega.value());
}
