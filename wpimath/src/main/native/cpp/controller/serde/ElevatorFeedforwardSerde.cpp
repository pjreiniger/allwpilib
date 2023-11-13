// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/controller/ElevatorFeedforward.h"
#include "controller.pb.h"

using StructType = wpi::Struct<frc::ElevatorFeedforward>;

frc::ElevatorFeedforward StructType::Unpack(
    std::span<const uint8_t, StructType::kSize> data) {
  return {
    // wpi::UnpackStruct<double, 0>(data),
    // wpi::UnpackStruct<double, 0>(data),
    // wpi::UnpackStruct<double, 0>(data),
    // wpi::UnpackStruct<double, 0>(data),
  };
}

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::ElevatorFeedforward& value) {
    // wpi::PackStruct<0>(data, value.Ks());
    // wpi::PackStruct<0>(data, value.Kg());
    // wpi::PackStruct<0>(data, value.Kv());
    // wpi::PackStruct<0>(data, value.Ka());
}

google::protobuf::Message* wpi::Protobuf<frc::ElevatorFeedforward>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufElevatorFeedforward>(arena);
}

frc::ElevatorFeedforward wpi::Protobuf<frc::ElevatorFeedforward>::Unpack(
    const google::protobuf::Message& msg) {
//   auto m = static_cast<const wpi::proto::ProtobufElevatorFeedforward*>(&msg);
  return frc::ElevatorFeedforward{
    // m->ks(),
    // m->kg(),
    // m->kv(),
    // m->ka(),
  };
}

void wpi::Protobuf<frc::ElevatorFeedforward>::Pack(google::protobuf::Message* msg,
                                             const frc::ElevatorFeedforward& value) {
//   auto m = static_cast<wpi::proto::ProtobufElevatorFeedforward*>(msg);
    // m->set_ks(value.Ks());
    // m->set_kg(value.Kg());
    // m->set_kv(value.Kv());
    // m->set_ka(value.Ka());
}
