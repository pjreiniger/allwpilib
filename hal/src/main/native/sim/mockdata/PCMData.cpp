/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "../PortsInternal.h"
#include "PCMDataInternal.h"

using namespace hal;

namespace hal {
namespace init {
void InitializePCMData() {
  static PCMData spd[kNumPCMModules];
  ::hal::SimPCMData = spd;
}
}  // namespace init
}  // namespace hal

PCMData* hal::SimPCMData;
void PCMData::ResetData() {
  for (int i = 0; i < kNumSolenoidChannels; i++) {
    solenoidInitialized[i].Reset(false);
    solenoidOutput[i].Reset(false);
    solenoidDisplayName[i][0] = '\0';
  }
  compressorInitialized.Reset(false);
  compressorOn.Reset(false);
  closedLoopEnabled.Reset(true);
  pressureSwitch.Reset(false);
  compressorCurrent.Reset(0.0);
}

extern "C" {
void HALSIM_ResetPCMData(int32_t index) { SimPCMData[index].ResetData(); }

#define DEFINE_CAPI(TYPE, CAPINAME, LOWERNAME)                          \
  HAL_SIMDATAVALUE_DEFINE_CAPI(TYPE, HALSIM, PCM##CAPINAME, SimPCMData, \
                               LOWERNAME)

HAL_SIMDATAVALUE_DEFINE_CAPI_CHANNEL(HAL_Bool, HALSIM, PCMSolenoidInitialized,
                                     SimPCMData, solenoidInitialized)
HAL_SIMDATAVALUE_DEFINE_CAPI_CHANNEL(HAL_Bool, HALSIM, PCMSolenoidOutput,
                                     SimPCMData, solenoidOutput)
DEFINE_CAPI(HAL_Bool, CompressorInitialized, compressorInitialized)
DEFINE_CAPI(HAL_Bool, CompressorOn, compressorOn)
DEFINE_CAPI(HAL_Bool, ClosedLoopEnabled, closedLoopEnabled)
DEFINE_CAPI(HAL_Bool, PressureSwitch, pressureSwitch)
DEFINE_CAPI(double, CompressorCurrent, compressorCurrent)

const char* HALSIM_GetSolenoidDisplayName(int32_t index, int32_t channel) {
  if (SimPCMData[index].solenoidDisplayName[channel][0] != '\0') {
    return SimPCMData[index].solenoidDisplayName[channel];
  }

  std::snprintf(SimPCMData[index].solenoidDisplayName[channel], sizeof(SimPCMData[index].solenoidDisplayName[channel]),
                "Solenoid[%d]", channel);
  return SimPCMData[index].solenoidDisplayName[channel];
}

void HALSIM_SetSolenoidDisplayName(int32_t index, int32_t channel,
                                   const char* displayName) {
  std::cout << "Setting display name for "
            << "Solenoid"
            << ", port " << index << " -> " << displayName << std::endl;
  std::strncpy(SimPCMData[index].solenoidDisplayName[channel], displayName,
               sizeof(SimPCMData[index].solenoidDisplayName[channel]) - 1);
  *(std::end(SimPCMData[index].solenoidDisplayName[channel]) - 1) = '\0';
}

void HALSIM_GetPCMAllSolenoids(int32_t index, uint8_t* values) {
  auto& data = SimPCMData[index].solenoidOutput;
  uint8_t ret = 0;
  for (int i = 0; i < kNumSolenoidChannels; i++) {
    ret |= (data[i] << i);
  }
  *values = ret;
}

void HALSIM_SetPCMAllSolenoids(int32_t index, uint8_t values) {
  auto& data = SimPCMData[index].solenoidOutput;
  for (int i = 0; i < kNumSolenoidChannels; i++) {
    data[i] = (values & 0x1) != 0;
    values >>= 1;
  }
}

#define REGISTER(NAME) \
  SimPCMData[index].NAME.RegisterCallback(callback, param, initialNotify)

void HALSIM_RegisterPCMAllNonSolenoidCallbacks(int32_t index,
                                               HAL_NotifyCallback callback,
                                               void* param,
                                               HAL_Bool initialNotify) {
  REGISTER(compressorInitialized);
  REGISTER(compressorOn);
  REGISTER(closedLoopEnabled);
  REGISTER(pressureSwitch);
  REGISTER(compressorCurrent);
}

void HALSIM_RegisterPCMAllSolenoidCallbacks(int32_t index, int32_t channel,
                                            HAL_NotifyCallback callback,
                                            void* param,
                                            HAL_Bool initialNotify) {
  REGISTER(solenoidInitialized[channel]);
  REGISTER(solenoidOutput[channel]);
}
}  // extern "C"
