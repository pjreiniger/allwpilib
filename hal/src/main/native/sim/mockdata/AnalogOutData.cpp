/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "../PortsInternal.h"
#include "AnalogOutDataInternal.h"

using namespace hal;

namespace hal {
namespace init {
void InitializeAnalogOutData() {
  static AnalogOutData siod[kNumAnalogOutputs];
  ::hal::SimAnalogOutData = siod;
}
}  // namespace init
}  // namespace hal

AnalogOutData* hal::SimAnalogOutData;
void AnalogOutData::ResetData() {
  voltage.Reset(0.0);
  initialized.Reset(0);
  displayName[0] = '\0';
}

extern "C" {
void HALSIM_ResetAnalogOutData(int32_t index) {
  SimAnalogOutData[index].ResetData();
}

#define DEFINE_CAPI(TYPE, CAPINAME, LOWERNAME)                    \
  HAL_SIMDATAVALUE_DEFINE_CAPI(TYPE, HALSIM, AnalogOut##CAPINAME, \
                               SimAnalogOutData, LOWERNAME)

const char* HALSIM_GetAnalogOutDisplayName(int32_t index) {
  if (SimAnalogOutData[index].displayName[0] != '\0') {
    return SimAnalogOutData[index].displayName;
  }

  std::snprintf(SimAnalogOutData[index].displayName, sizeof(SimAnalogOutData[index].displayName), "AnalogOut [%d]",
                index);
  return SimAnalogOutData[index].displayName;
}
void HALSIM_SetAnalogOutDisplayName(int32_t index, const char* displayName) {
  std::cout << "Setting display name for "
            << "SimAnalogOutData"
            << ", port " << index << " -> " << displayName << std::endl;
  std::strncpy(SimAnalogOutData[index].displayName, displayName,
               sizeof(SimAnalogOutData[index].displayName) - 1);
  *(std::end(SimAnalogOutData[index].displayName) - 1) = '\0';
}
DEFINE_CAPI(double, Voltage, voltage)
DEFINE_CAPI(HAL_Bool, Initialized, initialized)

#define REGISTER(NAME) \
  SimAnalogOutData[index].NAME.RegisterCallback(callback, param, initialNotify)

void HALSIM_RegisterAnalogOutAllCallbacks(int32_t index,
                                          HAL_NotifyCallback callback,
                                          void* param, HAL_Bool initialNotify) {
  REGISTER(voltage);
  REGISTER(initialized);
}
}  // extern "C"
