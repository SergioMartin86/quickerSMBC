#pragma once

#include "../SMBCInstanceBase.hpp"
#include <string>
#include <vector>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>

extern bool loadRomImage(uint8_t* buffer, size_t size);
extern bool initializeVideo();
extern void finalizeVideo();
extern void reset();
extern uint32_t* getRenderBuffer();
extern size_t getRenderBufferSize();
extern void advanceFrame(bool buttonUp, bool buttonDown, bool buttonLeft, bool buttonRight, bool buttonA, bool buttonB, bool buttonSelect, bool buttonStart);
extern void renderFrame();
extern void enableRendering();
extern void disableRendering();
extern size_t getStateSize();
extern void saveState(uint8_t* buffer);
extern void loadState(const uint8_t* buffer);
extern uint8_t* getRamPointer();

namespace smbc
{

class EmuInstance : public EmuInstanceBase
{
 public:

 EmuInstance() : EmuInstanceBase()
 {
 }

 ~EmuInstance()
 {
 }

  virtual void initialize() override
  {
  }

  virtual bool loadROMImpl(const std::string &romFilePath) override
  {
    std::string romData;
    jaffarCommon::file::loadStringFromFile(romData, romFilePath);
    loadRomImage((uint8_t*)romData.data(), romData.size());
    return true;
  }

  void initializeVideoOutput() override
  {
    initializeVideo();
  }

  void finalizeVideoOutput() override
  {
    finalizeVideo();
  }

  void enableRendering() override
  {
    ::enableRendering();
  }

  void disableRendering() override
  {
    ::disableRendering();
  }

  void serializeState(jaffarCommon::serializer::Base& s) const override
  {
    ::saveState(s.getOutputDataBuffer());
    s.pushContiguous(nullptr, _stateSize);
  }

  void deserializeState(jaffarCommon::deserializer::Base& d) override
  {
    ::loadState(d.getInputDataBuffer());
    d.popContiguous(nullptr, _stateSize);
  }

  size_t getStateSizeImpl() const override
  {
    return ::getStateSize();
  }

  void updateRenderer() override
  {
    ::renderFrame();
  }

  inline size_t getDifferentialStateSizeImpl() const override { return getStateSizeImpl(); }

  void enableStateBlockImpl(const std::string& block) override
  {
    JAFFAR_THROW_LOGIC("State block name: '%s' not found.", block.c_str());
  }

  void disableStateBlockImpl(const std::string& block) override
  {
    JAFFAR_THROW_LOGIC("State block name: '%s' not found", block.c_str());
  }

  void doSoftReset() override
  {
    ::reset();
  }
  
  void doHardReset() override
  {
    ::reset();
  }


  std::string getCoreName() const override { return "Base SMBC"; }

  virtual uint8_t* getRamPointer() const { return ::getRamPointer(); }

  void advanceStateImpl(smbc::Controller controller) override
  {
    const auto& input = controller.getInput();

    advanceFrame
    (
      input.buttonUp,
      input.buttonDown,
      input.buttonLeft,
      input.buttonRight,
      input.buttonA,
      input.buttonB,
      input.buttonSelect,
      input.buttonStart
    );
  }

  private:

};

} // namespace smbc