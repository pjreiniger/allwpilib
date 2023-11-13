// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <wpi/protobuf/Protobuf.h>
#include <wpi/struct/Struct.h>

#include "frc/controller/SimpleMotorFeedforward.h"

template <>
struct wpi::Struct<frc::SimpleMotorFeedforward> {
  static constexpr std::string_view kTypeString = "struct:SimpleMotorFeedforward";
  static constexpr size_t kSize = 24;
  static constexpr std::string_view kSchema = "double ks;double kv;double ka";

  static frc::SimpleMotorFeedforward Unpack(std::span<const uint8_t, kSize> data);
  static void Pack(std::span<uint8_t, kSize> data,
                   const frc::SimpleMotorFeedforward& value);
};

template <>
struct WPILIB_DLLEXPORT wpi::Protobuf<frc::SimpleMotorFeedforward> {
  static google::protobuf::Message* New(google::protobuf::Arena* arena);
  static frc::SimpleMotorFeedforward Unpack(const google::protobuf::Message& msg);
  static void Pack(google::protobuf::Message* msg,
                   const frc::SimpleMotorFeedforward& value);
};