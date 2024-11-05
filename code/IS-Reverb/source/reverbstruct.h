//------------------------------------------------------------------------
// Copyright(c) 2024 fhw.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "myplugincids.h"

#include "dsp/delay.h"

namespace fhw {

//------------------------------------------------------------------------
//  CReverbStructure
//------------------------------------------------------------------------

// Have this be a "master class" which handles creating objects for each part of reverb
class CReverbStructure : public Steinberg::Vst::AudioEffect
{
public:
	CReverbStructure ();
	~CReverbStructure ();

	/** call CDelayLine from here to generate delay lines */
	void createDelayLines ();
	void processDelay ();

	/** Used to set delay lines (9/23/24)*/
	// probably move to reverb structure
	Steinberg::tresult PLUGIN_API setBusArrangements (Steinberg::Vst::SpeakerArrangement* inputs, Steinberg::int32 numIns, Steinberg::Vst::SpeakerArrangement* outputs, Steinberg::int32 numOuts) SMTG_OVERRIDE;
	
	
protected:

};


class CDelayLine : public Steinberg::Vst::AudioEffect
{
public:
	CDelayLine ();
	~CDelayLine ();

	// set / get values for each line?
	Steinberg::tresult PLUGIN_API createLine ();

protected:

};


//------------------------------------------------------------------------
} // namespace fhw