// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "controller.pb.h"
#include "frc/controller/ArmFeedforward.h"

using StructType = wpi::Struct<frc::ArmFeedforward>;

frc::ArmFeedforward StructType::Unpack(
    std::span<const uint8_t, StructType::kSize> data) {
  return {
      units::volt_t{wpi::UnpackStruct<double, 0>(data)},
      units::volt_t{wpi::UnpackStruct<double, 8>(data)},
      units::unit_t<frc::ArmFeedforward::kv_unit>{
          wpi::UnpackStruct<double, 16>(data)},
      units::unit_t<frc::ArmFeedforward::ka_unit>{
          wpi::UnpackStruct<double, 24>(data)},
  };
}

void StructType::Pack(std::span<uint8_t, StructType::kSize> data,
                      const frc::ArmFeedforward& value) {
  wpi::PackStruct<0>(data, value.kS());
  wpi::PackStruct<8>(data, value.kG());
  wpi::PackStruct<16>(data, value.kV());
  wpi::PackStruct<24>(data, value.kA());
}

google::protobuf::Message* wpi::Protobuf<frc::ArmFeedforward>::New(
    google::protobuf::Arena* arena) {
  return google::protobuf::Arena::CreateMessage<
      wpi::proto::ProtobufArmFeedforward>(arena);
}

frc::ArmFeedforward wpi::Protobuf<frc::ArmFeedforward>::Unpack(
    const google::protobuf::Message& msg) {
  auto m = static_cast<const wpi::proto::ProtobufArmFeedforward*>(&msg);
  return frc::ArmFeedforward{
      units::volt_t{m->ks()},
      units::volt_t{m->kg()},
      units::unit_t<frc::ArmFeedforward::kv_unit>{m->kv()},
      units::unit_t<frc::ArmFeedforward::ka_unit>{m->ka()},
  };
}

void wpi::Protobuf<frc::ArmFeedforward>::Pack(
    google::protobuf::Message* msg, const frc::ArmFeedforward& value) {
  auto m = static_cast<wpi::proto::ProtobufArmFeedforward*>(msg);
  m->set_ks(value.kS.value());
  m->set_kg(value.kG.value());
  m->set_kv(value.kV.value());
  m->set_ka(value.kA.value());
}
