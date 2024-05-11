#ifndef SMBENGINE_HPP
#define SMBENGINE_HPP

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>

#include "../Emulation/MemoryAccess.hpp"

#include "SMBDataPointers.hpp"

#include "../Configuration.hpp"
#include "../Emulation/APU.hpp"
#include "../Emulation/Controller.hpp"
#include "../Emulation/PPU.hpp"

#include "SMBEngine.hpp"
#include <common.hpp>

#define DATA_STORAGE_OFFSET 0x8000 // Starting address for storing constant data

class APU;
class Controller;
class PPU;

/**
 * Engine that runs Super Mario Bros.
 * Handles emulation of various NES subsystems for compatibility and accuracy.
 */
class SMBEngine
{
    friend class MemoryAccess;
    friend class PPU;
public:
    SMBEngine(uint8_t* romImage) :
    a(*this, &registerA),
    x(*this, &registerX),
    y(*this, &registerY),
    s(*this, &registerS)
{
    apu = new APU();
    ppu = new PPU(*this);
    controller1 = new Controller();
    controller2 = new Controller();

    // CHR Location in ROM: Header (16 bytes) + 2 PRG pages (16k each)
    chr = (romImage + 16 + (16384 * 2));

    returnIndexStackTop = 0;
}

~SMBEngine()
{
    delete apu;
    delete ppu;
    delete controller1;
    delete controller2;
}

inline size_t getStateSize()
{
 size_t size = 0;
 size += sizeof(c);
 size += sizeof(z);
 size += sizeof(n);
 size += sizeof(registerA);
 size += sizeof(registerX);
 size += sizeof(registerY);
 size += sizeof(registerS);
 size += sizeof(a.constant);
 size += sizeof(x.constant);
 size += sizeof(y.constant);
 size += sizeof(s.constant);
 size += sizeof(dataStorage);
 size += sizeof(ram);
// size += sizeof(chr);
 size += sizeof(returnIndexStack);
 size += sizeof(returnIndexStackTop);
 size += sizeof(ppu->ppuCtrl);
 size += sizeof(ppu->ppuMask);
 size += sizeof(ppu->ppuStatus);
 size += sizeof(ppu->oamAddress);
 size += sizeof(ppu->ppuScrollX);
 size += sizeof(ppu->ppuScrollY);
 size += sizeof(ppu->palette);
 size += sizeof(ppu->nametable);
 size += sizeof(ppu->currentAddress);
 size += sizeof(ppu->writeToggle);
 size += sizeof(ppu->vramBuffer);
 return size;
}

inline void saveState(uint8_t* state) const
{
 uint8_t* ptr = state;
 memcpy(ptr, &c, sizeof(c)); ptr += sizeof(c);
 memcpy(ptr, &z, sizeof(z)); ptr += sizeof(z);
 memcpy(ptr, &n, sizeof(n)); ptr += sizeof(n);
 memcpy(ptr, &registerA, sizeof(registerA)); ptr += sizeof(registerA);
 memcpy(ptr, &registerX, sizeof(registerX)); ptr += sizeof(registerX);
 memcpy(ptr, &registerY, sizeof(registerY)); ptr += sizeof(registerY);
 memcpy(ptr, &registerS, sizeof(registerS)); ptr += sizeof(registerS);
 memcpy(ptr, &a.constant, sizeof(a.constant)); ptr += sizeof(a.constant);
 memcpy(ptr, &x.constant, sizeof(x.constant)); ptr += sizeof(x.constant);
 memcpy(ptr, &y.constant, sizeof(y.constant)); ptr += sizeof(y.constant);
 memcpy(ptr, &s.constant, sizeof(s.constant)); ptr += sizeof(s.constant);
 memcpy(ptr, &dataStorage, sizeof(dataStorage)); ptr += sizeof(dataStorage);
 memcpy(ptr, &ram, sizeof(ram)); ptr += sizeof(ram);
// memcpy(ptr, &chr, sizeof(chr)); ptr += sizeof(chr);
 memcpy(ptr, &returnIndexStack, sizeof(returnIndexStack)); ptr += sizeof(returnIndexStack);
 memcpy(ptr, &returnIndexStackTop, sizeof(returnIndexStackTop)); ptr += sizeof(returnIndexStackTop);
 memcpy(ptr, &ppu->ppuCtrl, sizeof(ppu->ppuCtrl)); ptr += sizeof(ppu->ppuCtrl);
 memcpy(ptr, &ppu->ppuMask, sizeof(ppu->ppuMask)); ptr += sizeof(ppu->ppuMask);
 memcpy(ptr, &ppu->ppuStatus, sizeof(ppu->ppuStatus)); ptr += sizeof(ppu->ppuStatus);
 memcpy(ptr, &ppu->oamAddress, sizeof(ppu->oamAddress)); ptr += sizeof(ppu->oamAddress);
 memcpy(ptr, &ppu->ppuScrollX, sizeof(ppu->ppuScrollX)); ptr += sizeof(ppu->ppuScrollX);
 memcpy(ptr, &ppu->ppuScrollY, sizeof(ppu->ppuScrollY)); ptr += sizeof(ppu->ppuScrollY);
 memcpy(ptr, &ppu->palette, sizeof(ppu->palette)); ptr += sizeof(ppu->palette);
 memcpy(ptr, &ppu->nametable, sizeof(ppu->nametable)); ptr += sizeof(ppu->nametable);
 memcpy(ptr, &ppu->currentAddress, sizeof(ppu->currentAddress)); ptr += sizeof(ppu->currentAddress);
 memcpy(ptr, &ppu->writeToggle, sizeof(ppu->writeToggle)); ptr += sizeof(ppu->writeToggle);
 memcpy(ptr, &ppu->vramBuffer, sizeof(ppu->vramBuffer)); ptr += sizeof(ppu->vramBuffer);
}

inline void loadState(const uint8_t* state)
{
 const uint8_t* ptr = state;
 memcpy(&c, ptr, sizeof(c)); ptr += sizeof(c);
 memcpy(&z, ptr, sizeof(z)); ptr += sizeof(z);
 memcpy(&n, ptr, sizeof(n)); ptr += sizeof(n);
 memcpy(&registerA, ptr, sizeof(registerA)); ptr += sizeof(registerA);
 memcpy(&registerX, ptr, sizeof(registerX)); ptr += sizeof(registerX);
 memcpy(&registerY, ptr, sizeof(registerY)); ptr += sizeof(registerY);
 memcpy(&registerS, ptr, sizeof(registerS)); ptr += sizeof(registerS);
 memcpy(&a.constant, ptr, sizeof(a.constant)); ptr += sizeof(a.constant);
 memcpy(&x.constant, ptr, sizeof(x.constant)); ptr += sizeof(x.constant);
 memcpy(&y.constant, ptr, sizeof(y.constant)); ptr += sizeof(y.constant);
 memcpy(&s.constant, ptr, sizeof(s.constant)); ptr += sizeof(s.constant);
 memcpy(&dataStorage, ptr, sizeof(dataStorage)); ptr += sizeof(dataStorage);
 memcpy(&ram, ptr, sizeof(ram)); ptr += sizeof(ram);
// memcpy(&chr, ptr, sizeof(chr)); ptr += sizeof(chr);
 memcpy(&returnIndexStack, ptr, sizeof(returnIndexStack)); ptr += sizeof(returnIndexStack);
 memcpy(&returnIndexStackTop, ptr, sizeof(returnIndexStackTop)); ptr += sizeof(returnIndexStackTop);
 memcpy(&ppu->ppuCtrl, ptr, sizeof(ppu->ppuCtrl)); ptr += sizeof(ppu->ppuCtrl);
 memcpy(&ppu->ppuMask, ptr, sizeof(ppu->ppuMask)); ptr += sizeof(ppu->ppuMask);
 memcpy(&ppu->ppuStatus, ptr, sizeof(ppu->ppuStatus)); ptr += sizeof(ppu->ppuStatus);
 memcpy(&ppu->oamAddress, ptr, sizeof(ppu->oamAddress)); ptr += sizeof(ppu->oamAddress);
 memcpy(&ppu->ppuScrollX, ptr, sizeof(ppu->ppuScrollX)); ptr += sizeof(ppu->ppuScrollX);
 memcpy(&ppu->ppuScrollY, ptr, sizeof(ppu->ppuScrollY)); ptr += sizeof(ppu->ppuScrollY);
 memcpy(&ppu->palette, ptr, sizeof(ppu->palette)); ptr += sizeof(ppu->palette);
 memcpy(&ppu->nametable, ptr, sizeof(ppu->nametable)); ptr += sizeof(ppu->nametable);
 memcpy(&ppu->currentAddress, ptr, sizeof(ppu->currentAddress)); ptr += sizeof(ppu->currentAddress);
 memcpy(&ppu->writeToggle, ptr, sizeof(ppu->writeToggle)); ptr += sizeof(ppu->writeToggle);
 memcpy(&ppu->vramBuffer, ptr, sizeof(ppu->vramBuffer)); ptr += sizeof(ppu->vramBuffer);
}

inline void audioCallback(uint8_t* stream, int length)
{
    apu->output(stream, length);
}

inline Controller& getController1()
{
    return *controller1;
}

inline Controller& getController2()
{
    return *controller2;
}

inline void render(uint32_t* buffer)
{
    ppu->render(buffer);
}

inline void reset()
{
    // Run the decompiled code for initialization
    code(0);
}

inline void update()
{
    // Run the decompiled code for the NMI handler
    code(1);

    // Update the APU
    if (Configuration::getAudioEnabled())
    {
        apu->stepFrame();
    }
}

    uint8_t* getRamPointer() { return ram; }
    
//---------------------------------------------------------------------
// Private methods
//---------------------------------------------------------------------

private:
    // NES Emulation subsystems:
    APU* apu;
    PPU* ppu;
    Controller* controller1;
    Controller* controller2;

    // Fields for NES CPU emulation:
    uint8_t registerA;           /**< Accumulator register. */
    uint8_t registerX;           /**< X index register. */
    uint8_t registerY;           /**< Y index register. */
    uint8_t registerS;           /**< Stack index register. */
    MemoryAccess a;              /**< Wrapper for A register. */
    MemoryAccess x;              /**< Wrapper for X register. */
    MemoryAccess y;              /**< Wrapper for Y register. */
    MemoryAccess s;              /**< Wrapper for S register. */
    uint8_t dataStorage[0x8000]; /**< 32kb of storage for constant data. */
    uint8_t ram[0x800];          /**< 2kb of RAM. */
    uint8_t* chr;                /**< Pointer to CHR data from the ROM. */
    int returnIndexStack[100];   /**< Stack for managing JSR subroutines. */
    int returnIndexStackTop;     /**< Current index of the top of the call stack. */

    // Pointers to constant data used in the decompiled code
    //
    SMBDataPointers dataPointers;
    
        /**
     * Load all constant data that was present in the SMB ROM.
     * 
     * See SMBData.cpp for implementation.
     */
void loadConstantData();

/**
 * Run the decompiled code for the game.
 * 
 * See SMB.cpp for implementation.
 *
 * @param mode the mode to run. 0 runs initialization routines, 1 runs the logic for frames.
 */
void code(int mode);

inline void compare(uint8_t value1, uint8_t value2)
{
    uint8_t result = value1 - value2;
    c = (value1 >= value2);
    setZN(result);
}

inline void bit(uint8_t value)
{
    n = (value & (1 << 7)) != 0;
    z = (registerA & value) == 0;
}

inline uint8_t* getCHR()
{
    return chr;
}

inline uint8_t* getDataPointer(uint16_t address)
{
    // Constant data
    if( address >= DATA_STORAGE_OFFSET )
    {
        return dataStorage + (address - DATA_STORAGE_OFFSET);
    }
    // RAM and Mirrors
    else if( address < 0x2000 )
    {
        return ram + (address & 0x7ff);
    }

    return nullptr;
}

inline MemoryAccess getMemory(uint16_t address)
{
    uint8_t* dataPointer = getDataPointer(address);
    if( dataPointer != nullptr )
    {
        return MemoryAccess(*this, dataPointer);
    }
    else
    {
        return MemoryAccess(*this, readData(address));
    }
}

inline uint16_t getMemoryWord(uint8_t address)
{
    return (uint16_t)readData(address) + ((uint16_t)(readData(address + 1)) << 8);
}

inline void pha()
{
    writeData(0x100 | (uint16_t)registerS, registerA);
    registerS--;
}

inline void pla()
{
    registerS++;
    a = readData(0x100 | (uint16_t)registerS);
}

inline int popReturnIndex()
{
    return returnIndexStack[returnIndexStackTop--];
}

inline void pushReturnIndex(int index)
{
    returnIndexStack[++returnIndexStackTop] = index;
}

inline uint8_t readData(uint16_t address)
{
    // Constant data
    if( address >= DATA_STORAGE_OFFSET )
    {
        return dataStorage[address - DATA_STORAGE_OFFSET];
    }
    // RAM and Mirrors
    else if( address < 0x2000 )
    {
        return ram[address & 0x7ff];
    }
    // PPU Registers and Mirrors
    else if( address < 0x4000 )
    {
        return ppu->readRegister(0x2000 + (address & 0x7));
    }
    // IO registers
    else if( address < 0x4020 )
    {
        switch (address)
        {
        case 0x4016:
            return controller1->readByte();
        case 0x4017:
            return controller2->readByte();
        }
    }

    return 0;
}

inline void writeData(uint16_t address, uint8_t value)
{
    // RAM and Mirrors
    if( address < 0x2000 )
    {
        ram[address & 0x7ff] = value;
    }
    // PPU Registers and Mirrors
    else if( address < 0x4000 )
    {
        ppu->writeRegister(0x2000 + (address & 0x7), value);
    }
    // IO registers
    else if( address < 0x4020 )
    {
        switch( address )
        {
        case 0x4014:
            ppu->writeDMA(value);
            break;
        case 0x4016:
            controller1->writeByte(value);
            controller2->writeByte(value);
            break;
        default:
            apu->writeRegister(address, value);
            break;
        }
    }
}

inline void writeData(uint16_t address, const uint8_t* data, size_t length)
{
    address -= DATA_STORAGE_OFFSET;

    memcpy(dataStorage + (std::ptrdiff_t)address, data, length);
}

};

#endif // SMBENGINE_HPP
