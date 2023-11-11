// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.math.geometry.serde;

import edu.wpi.first.math.proto.Geometry3D.ProtobufTwist3d;
import edu.wpi.first.util.protobuf.Protobuf;
import edu.wpi.first.math.geometry.Twist3d;
import us.hebi.quickbuf.Descriptors.Descriptor;

public class Twist3dProtoSerde implements Protobuf<Twist3d, ProtobufTwist3d> {
    @Override
    public Class<Twist3d> getTypeClass() {
      return Twist3d.class;
    }

    @Override
    public Descriptor getDescriptor() {
      return ProtobufTwist3d.getDescriptor();
    }

    @Override
    public ProtobufTwist3d createMessage() {
      return ProtobufTwist3d.newInstance();
    }

    @Override
    public Twist3d unpack(ProtobufTwist3d msg) {
      return new Twist3d(
          msg.getDxMeters(), msg.getDyMeters(), msg.getDzMeters(), msg.getRxRadians(), msg.getRyRadians(), msg.getRzRadians());
    }

    @Override
    public void pack(ProtobufTwist3d msg, Twist3d value) {
      msg.setDxMeters(value.dx)
          .setDyMeters(value.dy)
          .setDzMeters(value.dz)
          .setRxRadians(value.rx)
          .setRyRadians(value.ry)
          .setRzRadians(value.rz);
    }
}
