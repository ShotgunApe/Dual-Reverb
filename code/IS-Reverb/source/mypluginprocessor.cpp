//------------------------------------------------------------------------
// Copyright(c) 2024 fhw.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace fhw {
//------------------------------------------------------------------------
// CReverbProcessor
//------------------------------------------------------------------------
CReverbProcessor::CReverbProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kCReverbControllerUID);
}

//------------------------------------------------------------------------
CReverbProcessor::~CReverbProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId ())
                {
                    // for each input, include a case that check if a parameter has changed
                    case ReverbParams::kParamReverbGainId:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                            mReverbGain = value;
                    break;
				}
			}
		}
    }

    if (data.numInputs == 0 || data.numSamples == 0) {
        return kResultOk;
    }

	//--- Here you have to implement your processing
    int32 numChannels = data.inputs[0].numChannels;

    uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
    void** in = getChannelBuffersPointer(processSetup, data.inputs[0]);
    void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);

    data.outputs[0].silenceFlags = 0;

    float gain = mReverbGain;

    // Apply gain to each channel
    for (int32 i = 0; i < numChannels; i++) {
        int32 samples = data.numSamples;
        Vst::Sample32* ptrIn = (Vst::Sample32*)in[i];
        Vst::Sample32* ptrOut = (Vst::Sample32*)out[i];
        Vst::Sample32 tmp;

        while(--samples >= 0) {
            //apply gain
            tmp = (*ptrIn++) * gain;
            (*ptrOut++) = tmp;
        }

    }

    // Check if input is silent
    if (data.inputs[0].silenceFlags != 0) {
        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;

        for (int32 i = 0; i < numChannels; i++) {
            if (in[i] != out[i]) {
                memset (out[i], 0, sampleFramesSize);
            }
        }
    }

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::setState (IBStream* state)
{
    if(!state) {
        return kResultFalse;
    }

	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
    float savedParam1 = 0.f;
    if (streamer.readFloat(savedParam1) == false)
        return kResultFalse;

    mReverbGain = savedParam1;
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::getState (IBStream* state)
{
	// here we need to save the model
    float toSaveParam1 = mReverbGain;
	IBStreamer streamer (state, kLittleEndian);
    streamer.writeFloat(toSaveParam1);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace fhw
