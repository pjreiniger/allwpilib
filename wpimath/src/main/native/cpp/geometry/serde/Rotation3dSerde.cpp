// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/geometry/Rotation3d.h"
#include "geometry3d.pb.h"

using StructType = wpi::Struct<frc::Rotation3d>;

frc::Rotation3d StructType::Unpack(std::span<const uint8_t, kSize> data) {
  return frc::Rotation3d{wpi::UnpackStruct<frc::Quaternion, 0>(data)};
}
void StructType::Pack(std::span<uint8_t, kSize> data,
                      const frc::Rotation3d& value) {
  wpi::PackStruct<0>(data, value.GetQuaternion());
}
void StructType::ForEachNested(
    std::invocable<std::string_view, std::string_view> auto fn) {
  wpi::ForEachStructSchema<frc::Quaternion>(fn);
}

google::protobuf::Message* wpi::Protobuf<frc::Rotation3d>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<wpi::proto::ProtobufRotation3d>(
      arena);
}

frc::Rotation3d wpi::Protobuf<frc::Rotation3d>::Unpack(
    const google::protobuf::Message& msg) {
  auto m = static_cast<const wpi::proto::ProtobufRotation3d*>(&msg);
  return frc::Rotation3d{wpi::UnpackProtobuf<frc::Quaternion>(m->q())};
}

void wpi::Protobuf<frc::Rotation3d>::Pack(google::protobuf::Message* msg,
                                          const frc::Rotation3d& value) {
  auto m = static_cast<wpi::proto::ProtobufRotation3d*>(msg);
  wpi::PackProtobuf(m->mutable_q(), value.GetQuaternion());
}
