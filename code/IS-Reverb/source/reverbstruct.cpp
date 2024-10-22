//------------------------------------------------------------------------
// Copyright(c) 2024 fhw.
//------------------------------------------------------------------------

#include "reverbstruct.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace fhw {

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbStructure::setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs, Steinberg::int32 numIns, 
                                                        Steinberg::Vst::SpeakerArrangement* outputs, Steinberg::int32 numOuts) {
    if (numIns == 1 && numOuts == 1 && inputs[0 == outputs[0]])
        return Steinberg::Vst::AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
    return kResultFalse;
}


////////////////////////////////////////





}