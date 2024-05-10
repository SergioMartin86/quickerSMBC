#pragma once

#include "../SMBCInstanceBase.hpp"
#include <string>
#include <vector>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>


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
    return true;
  }

  void initializeVideoOutput() override
  {
  }

  void finalizeVideoOutput() override
  {
  }

  void enableRendering() override
  {
  }

  void disableRendering() override
  {
  }

  void serializeState(jaffarCommon::serializer::Base& s) const override
  {
  }

  void deserializeState(jaffarCommon::deserializer::Base& d) override
  {
  }

  size_t getStateSizeImpl() const override
  {
    return 0;
  }

  uint8_t* getRamPointer() const override
  {
    return nullptr;
  }

  void updateRenderer() override
  {
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
  }
  
  void doHardReset() override
  {
  }

  std::string getCoreName() const override { return "New SMBC"; }


  void advanceStateImpl(smbc::Controller controller) override
  {
  }

  private:

};

} // namespace smbc