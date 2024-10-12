#pragma once

#include <jaffarCommon/hash.hpp>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>
#include "inputParser.hpp"

namespace smbc
{

class EmuInstanceBase
{
  public:

  EmuInstanceBase(const nlohmann::json &config)
  {
    _inputParser = std::make_unique<jaffar::InputParser>(config);
  }

  virtual ~EmuInstanceBase() = default;

  virtual void advanceState(const jaffar::input_t &input)
  {
    advanceStateImpl(input);
  }

  inline jaffarCommon::hash::hash_t getStateHash() const
  {
    MetroHash128 hash;
    
    auto workRam = getRamPointer();

    hash.Update(workRam, 0x800);

    jaffarCommon::hash::hash_t result;
    hash.Finalize(reinterpret_cast<uint8_t *>(&result));
    return result;
  }

  virtual void initialize() = 0;
  virtual void initializeVideoOutput() = 0;
  virtual void finalizeVideoOutput() = 0;
  virtual void enableRendering() = 0;
  virtual void disableRendering() = 0;

  inline void loadROM(const std::string &romFilePath)
  {
    // Actually loading rom file
    auto status = loadROMImpl(romFilePath);
    if (status == false) JAFFAR_THROW_RUNTIME("Could not process ROM file");

    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  void enableStateBlock(const std::string& block) 
  {
    enableStateBlockImpl(block);
    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  void disableStateBlock(const std::string& block)
  {
     disableStateBlockImpl(block);
    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  inline size_t getStateSize() const 
  {
    return _stateSize;
  }

  inline size_t getDifferentialStateSize() const
  {
    return _differentialStateSize;
  }

  inline jaffar::InputParser *getInputParser() const { return _inputParser.get(); }
  
  // Virtual functions

  virtual void updateRenderer() = 0;
  virtual void serializeState(jaffarCommon::serializer::Base& s) const = 0;
  virtual void deserializeState(jaffarCommon::deserializer::Base& d) = 0;

  virtual void doSoftReset() = 0;
  virtual void doHardReset() = 0;
  virtual std::string getCoreName() const = 0;

  protected:

  virtual bool loadROMImpl(const std::string &romData) = 0;
  virtual void advanceStateImpl(const jaffar::input_t &input) = 0;

  virtual void enableStateBlockImpl(const std::string& block) {};
  virtual void disableStateBlockImpl(const std::string& block) {};

  virtual size_t getStateSizeImpl() const = 0;
  virtual size_t getDifferentialStateSizeImpl() const = 0;
  
  virtual uint8_t* getRamPointer() const = 0;

  // State size
  size_t _stateSize;

  private:

  // Differential state size
  size_t _differentialStateSize;

  // Input parser instance
  std::unique_ptr<jaffar::InputParser> _inputParser;
};

} // namespace smbc