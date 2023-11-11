// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

template <>
struct wpi::Struct<frc::Transform3d> {
  static constexpr std::string_view kTypeString = "struct:Transform3d";
  static constexpr size_t kSize = wpi::Struct<frc::Translation3d>::kSize +
                                  wpi::Struct<frc::Rotation3d>::kSize;
  static constexpr std::string_view kSchema =
      "Translation3d translation;Rotation3d rotation";

  static frc::Transform3d Unpack(std::span<const uint8_t, kSize> data);
  static void Pack(std::span<uint8_t, kSize> data,
                   const frc::Transform3d& value);
  static void ForEachNested(
      std::invocable<std::string_view, std::string_view> auto fn);
};

static_assert(wpi::HasNestedStruct<frc::Transform3d>);

template <>
struct WPILIB_DLLEXPORT wpi::Protobuf<frc::Transform3d> {
  static google::protobuf::Message* New(google::protobuf::Arena* arena);
  static frc::Transform3d Unpack(const google::protobuf::Message& msg);
  static void Pack(google::protobuf::Message* msg,
                   const frc::Transform3d& value);
};