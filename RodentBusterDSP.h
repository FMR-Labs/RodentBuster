#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "../FMR_Library/FMR_Lib/FMR_DSP_float.h"
#include "../FMR_Library/FMR_Pedal/IPedal.h"

#include "../FMR_Library/FMR_Pedal/PedalConst.h"

#include <atomic>
#include <memory>
#include <array>

enum RdBr_EParams
{
	RdBr_kGain = 0,
	RdBr_kTone,
	RdBr_kVolume,
	RdBr_kFilter,
	RdBr_kBoost,
	RdBr_kFootSw,
	RdBr_kNumParams
};

enum RdBr_EControlTags
{
	RdBr_kCtrlInputLED = 0,
	RdBr_kCtrlOutputLED,
	RdBr_kCtrlLED,
	RdBr_kNumLeds
};

template <typename T> class FMR_RateAdapter;
class ND_RodentBuster;
class ND_Boost;

class RodentBusterDSP : public IPedal
{
public:
  struct Parameters
  {
    float gain = 0.3f;
    float tone = 0.666f;
    float volume = 0.5f;
    int filter = 1;
    float boost = 0.5f;
    int footSw = 1;
  };

  RodentBusterDSP();
  ~RodentBusterDSP();

  RodentBusterDSP(const RodentBusterDSP&) = delete;
  RodentBusterDSP& operator=(const RodentBusterDSP&) = delete;

  void Reset(double sampleRate, int blockSize);

  void ProcessBlock(iplug::sample** inputs, iplug::sample** outputs, int nFrames, int nChans);

  int GetLatency() const;

  // IPedal (utilisé par le board : moteur seul, 48 kHz, en place)
  const char* Id() const override { return "RodentBuster"; }
  
  int NumParams() const override { return RdBr_kNumParams; }
  int NumLeds() const override { return RdBr_kNumLeds; }

  const char* ParamName(int k) const override;
  float ParamDefault(int k) const override;
  void SetParam(int k, float norm) override;
  void ResetEngine() override;
  void Process(iplug::sample** io, int nFrames, int nChans) override
  {
    ProcessEngine(io, io, nFrames, nChans, mParams);
  }

  void SetLed(int led, float color, float value, int decay);
  float GetLedColor(int led);
  float GetLedValue(int led);
  int GetLedDecay(int led);
  
  void setDisplayedLed(bool value) {displayedLed = value;}

private:
  void ProcessEngine(iplug::sample** inputs, iplug::sample** outputs, int nFrames, int nChans, const Parameters& params);
  void levelColorLed(int led, float level, float fullScale, float greenAt, float orangeAt);

  Parameters mParams;
  
  std::unique_ptr<ND_RodentBuster> mDrive;
  std::unique_ptr<ND_Boost> mBoost;
  std::unique_ptr<FMR_RateAdapter<iplug::sample>> mRate;
  
  bool displayedLed = false;

  FilterFloat_t filterGain{};
  FilterFloat_t filterTone{};
  FilterFloat_t filterVolume{};

  float currentGain = 0.0f;
  float currentTone = 0.0f;
  float currentVolume = 0.0f;

  struct myLed
  {
      float color = 0.0f;
      float value = 0.0f;
      int decay = 1;
  };
  std::array<myLed, RdBr_kNumLeds> pedalLed;
};
