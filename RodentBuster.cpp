
#include "RodentBuster.h"

#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include "RodentBusterGUI.h"
#include "../FMR_Library/FMR_NeuralDrive/FMR_RodentBuster.h"
#include "../FMR_Library/FMR_Lib/FMR_ConvPot.h"

#include <algorithm>

// utiliser struct Parameters et RodentBusterDSP::SetParam pour ne pas dupliquer les infos ?
RodentBuster::RodentBuster(const InstanceInfo& info) : iplug::Plugin(info, MakeConfig(RdBr_kNumParams, kNumPresets))
{
  GetParam(RdBr_kGain)->InitDouble("Gain", 0.3, 0.1, 0.999, 0.001);
  GetParam(RdBr_kTone)->InitInt("Tone", 2730, 0, 4095);
  GetParam(RdBr_kVolume)->InitInt("Volume", 2048, 0, 4095);

  GetParam(RdBr_kFilter)->InitInt("Filter", 1, 0, 2);
  GetParam(RdBr_kBoost)->InitDouble("Boost", 0.5f, 0.001f, 0.999f, 0.499f);
  
  GetParam(RdBr_kFootSw)->InitInt("Foot Switch", 1, 0, 1);
  
#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]()
  {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics)
  {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    RodentBusterGUI::Build(pGraphics, 0.f, 0.f, 0, true);
	mDSP.setDisplayedLed(true);
  };
#endif
}

void RodentBuster::OnIdle()
{
#if IPLUG_EDITOR
  if (!GetUI()) return;
  
  for(int l = 0; l < RdBr_kNumLeds ; l++ )
  {
    if ( auto* led = dynamic_cast<ILEDControl*>( GetUI()->GetControlWithTag( kNumStartLED + l) ) )
	{
      float value = GetBypassed() ? 0.0f : mDSP.GetLedValue(l);
      led->SetValue(value);
      if (value != 0.0f)
      {
          led->TriggerWithDecay(mDSP.GetLedDecay(l));
      }
      led->SetHue(mDSP.GetLedColor(l));
      led->SetDirty(true);
    }
  }

#endif
}

#if IPLUG_DSP

void RodentBuster::OnReset()
{
  mDSP.Reset(GetSampleRate(), std::max(GetBlockSize(), 64));
  SetLatency(mDSP.GetLatency());
}

void RodentBuster::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = std::min(NOutChansConnected(),ND_RODENT_BUSTER_NB_CHANNELS);

  // Conversion unique : RodentBusterDSP::SetParam (partagée avec le board).
  // lecture de tous les knobs et conversion (mise à l'échelle)
  for (int k = 0; k < mDSP.NumParams(); k++)
    mDSP.SetParam(k, static_cast<float>(GetParam(k)->GetNormalized()));

  mDSP.ProcessBlock(inputs, outputs, nFrames, nChans);  
}

#endif
