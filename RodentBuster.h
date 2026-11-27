#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "RodentBusterDSP.h"

using namespace iplug;
using namespace igraphics;

const int kNumPresets = 1;

class RodentBuster final : public Plugin
{
public:
  RodentBuster(const InstanceInfo& info);
  ~RodentBuster() override = default;

  void OnIdle() override;

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;

private:
  RodentBusterDSP mDSP;
#endif
};
