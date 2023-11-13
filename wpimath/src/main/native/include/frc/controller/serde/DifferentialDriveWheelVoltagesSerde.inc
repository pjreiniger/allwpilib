// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <wpi/protobuf/Protobuf.h>
#include <wpi/struct/Struct.h>

#include <wpi/SymbolExports.h>

#include "frc/controller/DifferentialDriveWheelVoltages.h"

template <>
struct wpi::Struct<frc::DifferentialDriveWheelVoltages> {
  static constexpr std::string_view kTypeString = "struct:DifferentialDriveWheelVoltages";
  static constexpr size_t kSize = 16;
  static constexpr std::string_view kSchema = "double left;double right";

  static frc::DifferentialDriveWheelVoltages Unpack(std::span<const uint8_t, kSize> data);
  static void Pack(std::span<uint8_t, kSize> data,
                   const frc::DifferentialDriveWheelVoltages& value);
};

template <>
struct WPILIB_DLLEXPORT wpi::Protobuf<frc::DifferentialDriveWheelVoltages> {
  static google::protobuf::Message* New(google::protobuf::Arena* arena);
  static frc::DifferentialDriveWheelVoltages Unpack(const google::protobuf::Message& msg);
  static void Pack(google::protobuf::Message* msg,
                   const frc::DifferentialDriveWheelVoltages& value);
};