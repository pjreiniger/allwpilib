/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "../PortsInternal.h"
#include "DIODataInternal.h"
#include "EncoderDataInternal.h"

using namespace hal;

namespace hal {
namespace init {
void InitializeDIOData() {
  static DIOData sdd[kNumDigitalChannels];
  ::hal::SimDIOData = sdd;
}
}  // namespace init
}  // namespace hal

DIOData* hal::SimDIOData;
void DIOData::ResetData() {
  initialized.Reset(false);
  simDevice = 0;
  value.Reset(true);
  pulseLength.Reset(0.0);
  isInput.Reset(true);
  filterIndex.Reset(-1);
  displayName.Reset();
}

extern "C" {
void HALSIM_ResetDIOData(int32_t index) { SimDIOData[index].ResetData(); }

HAL_SimDeviceHandle HALSIM_GetDIOSimDevice(int32_t index) {
  return SimDIOData[index].simDevice;
}

#define DEFINE_CAPI(TYPE, CAPINAME, LOWERNAME)                          \
  HAL_SIMDATAVALUE_DEFINE_CAPI(TYPE, HALSIM, DIO##CAPINAME, SimDIOData, \
                               LOWERNAME)

const char* HALSIM_GetDIODisplayName(int32_t index) {
  // if (SimDIOData[index].displayName[0] != '\0') {
  //   return SimDIOData[index].displayName;
  // }

  // std::cout << "Getting DIO name" << std::endl;
  
  // for (int i = 0; i < kNumEncoders; ++i) {
  //   std::cout << "Checking enc: " << i << std::endl;
  //   int encoderChannel = -1;
  //   if (HALSIM_GetEncoderInitialized(i)) {
  //     int channel;
  //     channel = HALSIM_GetEncoderDigitalChannelA(i);
  //     if (channel >= 0 && channel < kNumDigitalChannels) {
  //       encoderChannel = i;
  //       std::cout << "  Got something? " << encoderChannel << std::endl;
  //     } 
  //     channel = HALSIM_GetEncoderDigitalChannelB(i);
  //     if (channel >= 0 && channel < kNumDigitalChannels) {
  //       encoderChannel = i;
  //       std::cout << "  Got something2? " << encoderChannel << std::endl;
  //     }
  //   }
  //   if(encoderChannel >= 0)
  //   {
  //     std::cout << "HHHHH " << std::endl;
  //     std::snprintf(SimDIOData[index].displayName, sizeof(SimDIOData[index].displayName), 
  //                         HALSIM_GetEncoderDisplayName(encoderChannel));
  //     return SimDIOData[index].displayName;
  //   }
  // }

  // if(HALSIM_GetDIOIsInput(index))
  // {
  //   std::snprintf(SimDIOData[index].displayName, sizeof(SimDIOData[index].displayName), "In [%d]", index);
  // }
  // else
  // {
  //   std::snprintf(SimDIOData[index].displayName, sizeof(SimDIOData[index].displayName), "Out [%d]", index);
  // }
  
  // return SimDIOData[index].displayName;
  
   return SimDIOData[index].displayName.Get([]() { return "DIO"; });
}
void HALSIM_SetDIODisplayName(int32_t index, const char* displayName) {
  // std::cout << "Setting display name for "
  //           << "DIO"
  //           << ", port " << index << " -> " << displayName << std::endl;
  // std::strncpy(SimDIOData[index].displayName, displayName,
  //              sizeof(SimDIOData[index].displayName) - 1);
  // *(std::end(SimDIOData[index].displayName) - 1) = '\0';
  
   SimDIOData[index].displayName.Set(displayName);
}

DEFINE_CAPI(HAL_Bool, Initialized, initialized)
DEFINE_CAPI(HAL_Bool, Value, value)
DEFINE_CAPI(double, PulseLength, pulseLength)
DEFINE_CAPI(HAL_Bool, IsInput, isInput)
DEFINE_CAPI(int32_t, FilterIndex, filterIndex)

#define REGISTER(NAME) \
  SimDIOData[index].NAME.RegisterCallback(callback, param, initialNotify)

void HALSIM_RegisterDIOAllCallbacks(int32_t index, HAL_NotifyCallback callback,
                                    void* param, HAL_Bool initialNotify) {
  REGISTER(initialized);
  REGISTER(value);
  REGISTER(pulseLength);
  REGISTER(isInput);
  REGISTER(filterIndex);
}
}  // extern "C"
