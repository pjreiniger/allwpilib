// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <wpi/protobuf/Protobuf.h>
#include <wpi/struct/Struct.h>

#include <wpi/SymbolExports.h>

#include "frc/trajectory/Trajectory.h"

template <>
struct WPILIB_DLLEXPORT wpi::Protobuf<frc::Trajectory> {
  static google::protobuf::Message* New(google::protobuf::Arena* arena);
  static frc::Trajectory Unpack(const google::protobuf::Message& msg);
  static void Pack(google::protobuf::Message* msg,
                   const frc::Trajectory& value);
};