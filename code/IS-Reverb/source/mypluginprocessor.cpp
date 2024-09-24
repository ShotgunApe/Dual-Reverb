//------------------------------------------------------------------------
// Copyright(c) 2024 fhw.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

#include <algorithm>
#include <cstdlib>

using namespace Steinberg;

namespace fhw {
//------------------------------------------------------------------------
// CReverbProcessor
//------------------------------------------------------------------------
CReverbProcessor::CReverbProcessor () : mBuffer (nullptr)
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
	//addEventInput (STR16 ("Event In"), 1);

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
tresult PLUGIN_API CReverbProcessor::setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs, Steinberg::int32 numIns, 
                                                        Steinberg::Vst::SpeakerArrangement* outputs, Steinberg::int32 numOuts) {
    if (numIns == 1 && numOuts == 1 && inputs[0 == outputs[0]])
        return Steinberg::Vst::AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
    return kResultFalse;

}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----

    Steinberg::Vst::SpeakerArrangement arr;
    if (getBusArrangement(Steinberg::Vst::kOutput, 0, arr) != kResultTrue)
        return kResultFalse;
    int32 numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(arr);
    if (numChannels == 0)
        return kResultFalse;

    if (state) {
        //dynamically allocate enough memory to hold buffer for each delay line (kill me)
        mBuffer = (float**)std::malloc(numChannels * sizeof (float*));

        size_t size = static_cast<size_t> (processSetup.sampleRate *sizeof (float) + 0.5); //idk why this is ill find out later
        for (int32 channel = 0; channel < numChannels; channel++) {
            mBuffer[channel] = (float*)std::malloc(size);
            if (mBuffer[channel])
                memset(mBuffer[channel], 0, size);
        }
        mBufferPos = 0;
    }
    else {
        if (mBuffer) {
            for (int32 channel = 0; channel < numChannels; channel++) {
                std::free(mBuffer[channel]);
            }
            std::free(mBuffer);
            mBuffer = nullptr;
        }
    }

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
                    case ReverbParams::kParamReverbDecayGainId:
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                            mReverbDecayGain = value;
                        break;
				}
			}
		}
    }

    if (data.numSamples > 0) {
        //--- Here you have to implement your processing
        Steinberg::Vst::SpeakerArrangement arr;
        getBusArrangement(Steinberg::Vst::kOutput, 0, arr);
        int32 numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(arr);

        float gain = mReverbGain;
        int32 delayInSamples = std::max<int32> (1, (int32) (mReverbDecayGain * processSetup.sampleRate));

        // Apply processing to each channel
        for (int32 i = 0; i < numChannels; i++) {
            float* inputChannel = data.inputs[0].channelBuffers32[i];
            float* outputChannel = data.outputs[0].channelBuffers32[i];

            int32 tempBufferPos = mBufferPos;
            for (int32 sample = 0; sample < data.numSamples; sample++) {
                float tempSample = inputChannel[sample];
                outputChannel[sample] = mBuffer[i][tempBufferPos] * gain; // tha big one
                mBuffer[i][tempBufferPos] = tempSample;
                tempBufferPos++;
                if (tempBufferPos >= delayInSamples)
                    tempBufferPos = 0;
            }
        }
        mBufferPos += data.numSamples;
        while (delayInSamples && mBufferPos >= delayInSamples)
            mBufferPos -= delayInSamples;
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
    float savedParam2 = 0.f;
    if (streamer.readFloat(savedParam1) == false)
        return kResultFalse;
    if (streamer.readFloat(savedParam2) == false)
        return kResultFalse;

    mReverbGain = savedParam1;
    mReverbDecayGain = savedParam2;
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CReverbProcessor::getState (IBStream* state)
{
	IBStreamer streamer (state, kLittleEndian);
    streamer.writeFloat(mReverbGain);
    streamer.writeFloat(mReverbDecayGain);
	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace fhw
