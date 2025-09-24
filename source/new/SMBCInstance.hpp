#pragma once

#include "../SMBCInstanceBase.hpp"
#include <string>
#include <vector>
#include <memory>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include "core/source/SMB/SMBEngine.hpp"

extern bool loadRomImage(uint8_t* buffer, size_t size);
extern bool initializeVideo();
extern void finalizeVideo();
extern uint32_t* getRenderBuffer();
extern size_t getRenderBufferSize();
extern void advanceFrame(SMBEngine* smbEngine, bool buttonUp, bool buttonDown, bool buttonLeft, bool buttonRight, bool buttonA, bool buttonB, bool buttonSelect, bool buttonStart);
extern void renderFrame();
extern void enableRendering();
extern void disableRendering();
extern size_t getStateSize();
extern void saveState(jaffarCommon::serializer::Base& s);
extern void loadState(jaffarCommon::deserializer::Base& d);
extern void enableStateBlock(const std::string& block);
extern void disableStateBlock(const std::string& block);
extern uint8_t* getVideoBufferPointer();
extern size_t getVideoBufferSize();
extern thread_local uint8_t* romImage;

namespace smbc
{

class EmuInstance : public EmuInstanceBase
{
 public:

 EmuInstance(const nlohmann::json &config) : EmuInstanceBase(config)
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

    // Read the entire file into a buffer
    romImage = new uint8_t[romData.size()];
    memcpy(romImage, (uint8_t*)romData.data(), romData.size());
    smbEngine = std::make_unique<SMBEngine>(romImage);
    smbEngine->reset();
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
    smbEngine->saveState(s);
  }

  void deserializeState(jaffarCommon::deserializer::Base& d) override
  {
    smbEngine->loadState(d);
  }

  size_t getStateSizeImpl() const override
  {
    jaffarCommon::serializer::Contiguous s;
    serializeState(s);
    return s.getOutputSize();
  }

  void updateRenderer() override
  {
    ::renderFrame();
  }

  inline size_t getDifferentialStateSizeImpl() const override { return getStateSizeImpl(); }

  void enableStateBlockImpl(const std::string& block) override
  {
    ::enableStateBlock(block);
  }

  void disableStateBlockImpl(const std::string& block) override
  {
    ::disableStateBlock(block);
  }

  void doSoftReset() override
  {
    smbEngine->reset();
  }
  
  void doHardReset() override
  {
    smbEngine->reset();
  }

  uint8_t* getVideoBufferPointer() const
  {
    return ::getVideoBufferPointer();
  }

  size_t getVideoBufferSize() const
  {
    return ::getVideoBufferSize();
  }

  std::string getCoreName() const override { return "QuickerSMBC"; }

  virtual uint8_t* getRamPointer() const { return smbEngine->getRamPointer(); }

  void advanceStateImpl(const jaffar::input_t &input) override
  {
    advanceFrame
    (
      smbEngine.get(),
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

  std::unique_ptr<SMBEngine> smbEngine;
};

} // namespace smbc