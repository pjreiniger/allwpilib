// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.simulation;

import edu.wpi.first.hal.SimDouble;
import edu.wpi.first.wpilibj.ADXL345_I2C;
import edu.wpi.first.wpilibj.ADXL345_SPI;

import java.util.Objects;

public class ADXL345Sim {
    protected SimDouble m_simX;
    protected SimDouble m_simY;
    protected SimDouble m_simZ;

    public ADXL345Sim(ADXL345_SPI device) {
        SimDeviceSim wrappedSimDevice = new SimDeviceSim("Accel:ADXL345_SPI" + "[" + device.getPort() + "]");
        initSim(wrappedSimDevice);
    }

    public ADXL345Sim(ADXL345_I2C device) {
        SimDeviceSim wrappedSimDevice = new SimDeviceSim("Accel:ADXL345_I2C" + "[" + device.getPort() + "," + device.getDeviceAddress() + "]");
        initSim(wrappedSimDevice);
    }

    private void initSim(SimDeviceSim wrappedSimDevice) {
        m_simX = wrappedSimDevice.getDouble("x");
        m_simY = wrappedSimDevice.getDouble("y");
        m_simZ = wrappedSimDevice.getDouble("z");

        Objects.requireNonNull(m_simX);
        Objects.requireNonNull(m_simY);
        Objects.requireNonNull(m_simZ);
    }

    public void setX(double x) {
        m_simX.set(x);
    }
    public void setY(double y) {
        m_simY.set(y);
    }
    public void setZ(double z) {
        m_simZ.set(z);
    }
}
