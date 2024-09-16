//------------------------------------------------------------------------
// Copyright(c) 2024 fhw.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace fhw {
//------------------------------------------------------------------------
static const Steinberg::FUID kCReverbProcessorUID (0x099A656F, 0xF6B1532D, 0xB9389F6F, 0x9E80B962);
static const Steinberg::FUID kCReverbControllerUID (0x162D94BD, 0x39A25C85, 0xA83E47D9, 0x4E7B2393);

#define CReverbVST3Category "Fx"

// Include enum with parameters for knobs, add more with time as functionality increases (9/16/24)
enum ReverbParams : Steinberg::Vst::ParamID {
    kParamReverbGainId = 102,
};

//------------------------------------------------------------------------
} // namespace fhw
