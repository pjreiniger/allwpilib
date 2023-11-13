// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <wpi/protobuf/Protobuf.h>
#include <wpi/struct/Struct.h>

#include <wpi/SymbolExports.h>

#include "frc/kinematics/SwerveModulePosition.h"

template <>
struct wpi::Struct<frc::SwerveModulePosition> {
  static constexpr std::string_view kTypeString = "struct:SwerveModulePosition";
  static constexpr size_t kSize = 8+ wpi::Struct<frc::Rotation2d>::kSize;
  static constexpr std::string_view kSchema = "double distance;Rotation2d angle";

  static frc::SwerveModulePosition Unpack(std::span<const uint8_t, kSize> data);
  static void Pack(std::span<uint8_t, kSize> data,
                   const frc::SwerveModulePosition& value);
  static void ForEachNested(
      std::invocable<std::string_view, std::string_view> auto fn);
};

static_assert(wpi::HasNestedStruct<frc::SwerveModulePosition>);

template <>
struct WPILIB_DLLEXPORT wpi::Protobuf<frc::SwerveModulePosition> {
  static google::protobuf::Message* New(google::protobuf::Arena* arena);
  static frc::SwerveModulePosition Unpack(const google::protobuf::Message& msg);
  static void Pack(google::protobuf::Message* msg,
                   const frc::SwerveModulePosition& value);
};