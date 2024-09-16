#pragma once

#include "pluginterfaces/vst/vsttypes.h"

// Include enum with parameters for knobs, add more with time as functionality increases (9/16/24)
enum ReverbParams : Steinberg::Vst::ParamID {
    kParamReverbGainId = 102,
};
