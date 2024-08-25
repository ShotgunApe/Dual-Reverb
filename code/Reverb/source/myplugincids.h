//------------------------------------------------------------------------
// Copyright(c) 2024 fhw.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace fhw {
//------------------------------------------------------------------------
static const Steinberg::FUID kCReverbProcessorUID (0xC3E0B4FE, 0x897150D8, 0x888FBADB, 0xF2EDFEF4);
static const Steinberg::FUID kCReverbControllerUID (0x0C3AED8A, 0xDA6B50F3, 0x9F96F1B0, 0xFDA682D0);

#define CReverbVST3Category "TestCategory"

//------------------------------------------------------------------------
} // namespace fhw
