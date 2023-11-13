// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/kinematics/SwerveModulePosition.h"
#include "kinematics.pb.h"

using StructType = wpi::Struct<frc::SwerveModulePosition>;

frc::SwerveModulePosition StructType::Unpack(
    std::span<const uint8_t, StructType::kSize> data) {
  return frc::SwerveModulePosition{
      units::meter_t{wpi::UnpackStruct<double, 0>(data)},
      wpi::UnpackStruct<frc::Rotation2d, 8>(data),
  };
}

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::SwerveModulePosition& value) {
  wpi::PackStruct<0>(data, value.distance.value());
  wpi::PackStruct<8>(data, value.angle);
}

void StructType::ForEachNested(
    std::invocable<std::string_view, std::string_view> auto fn) {
  wpi::ForEachStructSchema<frc::Rotation2d>(fn);
}

google::protobuf::Message* wpi::Protobuf<frc::SwerveModulePosition>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufSwerveModulePosition>(arena);
}

frc::SwerveModulePosition wpi::Protobuf<frc::SwerveModulePosition>::Unpack(
    const google::protobuf::Message& msg) {
  auto m = static_cast<const wpi::proto::ProtobufSwerveModulePosition*>(&msg);
  return frc::SwerveModulePosition{
      units::meter_t{m->distance_meters()},
      wpi::UnpackProtobuf<frc::Rotation2d>(m->angle()),
  };
}

void wpi::Protobuf<frc::SwerveModulePosition>::Pack(
    google::protobuf::Message* msg, const frc::SwerveModulePosition& value) {
  auto m = static_cast<wpi::proto::ProtobufSwerveModulePosition*>(msg);
  m->set_distance_meters(value.distance.value());
  wpi::PackProtobuf(m->mutable_angle(), value.angle);
}
