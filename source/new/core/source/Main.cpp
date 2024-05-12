#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>

#include "Emulation/Controller.hpp"
#include "SMB/SMBEngine.hpp"
#include "Util/Video.hpp"
#include "common.hpp"

#include "Constants.hpp"

#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/deserializers/base.hpp>

thread_local uint8_t* romImage;
thread_local SDL_Window* window;
thread_local SDL_Renderer* renderer;
thread_local SDL_Texture* texture;
thread_local SDL_Texture* scanlineTexture;
thread_local SMBEngine* smbEngine = nullptr;
thread_local uint32_t renderBuffer[RENDER_WIDTH * RENDER_HEIGHT];
thread_local bool _doRendering = false;

void enableStateBlock(const std::string& block)
{
  if (block == "PPU") { _storePPUEnabled = true; return; }
  if (block == "DST") { _storeDataStorageEnabled = true; return; }
  if (block == "STK") { _storeStackEnabled = true; return; }

  fprintf(stderr, "Property name: '%s' not recognized", block.c_str());
  exit(-1);
}

void disableStateBlock(const std::string& block)
{
  if (block == "PPU") { _storePPUEnabled = false; return; }
  if (block == "DST") { _storeDataStorageEnabled = false; return; }
  if (block == "STK") { _storeStackEnabled = false; return; }

  fprintf(stderr, "Property name: '%s' not recognized", block.c_str());
  exit(-1);
}

void enableRendering()
{
    _doRendering = true;
}

void disableRendering()
{
    _doRendering = false;
}

uint8_t* getVideoBufferPointer()
{
  return (uint8_t*)renderBuffer;
}

size_t getVideoBufferSize() 
{
  return RENDER_WIDTH * RENDER_HEIGHT * sizeof(uint32_t);
}

/**
 * Load the Super Mario Bros. ROM image.
 */
bool loadRomImage(uint8_t* buffer, size_t size)
{
    // Read the entire file into a buffer
    romImage = new uint8_t[size];
    memcpy(romImage, buffer, size);
    smbEngine = new SMBEngine(romImage);
    smbEngine->reset();
    return true;
}

uint8_t* getRamPointer()
{
  return smbEngine->getRamPointer();
}

/**
 * Initialize libraries for use.
 */
bool initializeVideo()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init() failed during initialize(): " << SDL_GetError() << std::endl;
        return false;
    }

    // Create the window
    window = SDL_CreateWindow(APP_TITLE,
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              RENDER_WIDTH,
                              RENDER_HEIGHT,
                              0);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow() failed during initialize(): " << SDL_GetError() << std::endl;
        return false;
    }

    // Setup the renderer and texture buffer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer() failed during initialize(): " << SDL_GetError() << std::endl;
        return false;
    }

    if (SDL_RenderSetLogicalSize(renderer, RENDER_WIDTH, RENDER_HEIGHT) < 0)
    {
        std::cout << "SDL_RenderSetLogicalSize() failed during initialize(): " << SDL_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);
    if (texture == nullptr)
    {
        std::cout << "SDL_CreateTexture() failed during initialize(): " << SDL_GetError() << std::endl;
        return false;
    }


    return true;
}

/**
 * Shutdown libraries for exit.
 */
void finalizeVideo()
{
    SDL_DestroyTexture(scanlineTexture);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void reset()
{
  smbEngine->reset();
}

uint32_t* getRenderBuffer()
{
  return renderBuffer;
}

size_t getRenderBufferSize()
{
  return 61440 * sizeof(uint32_t);
}

void advanceFrame(bool buttonUp, bool buttonDown, bool buttonLeft, bool buttonRight, bool buttonA, bool buttonB, bool buttonSelect, bool buttonStart)
{
    Controller& controller1 = smbEngine->getController1();
    controller1.setButtonState(BUTTON_A, buttonA);
    controller1.setButtonState(BUTTON_B, buttonB);
    controller1.setButtonState(BUTTON_SELECT, buttonSelect);
    controller1.setButtonState(BUTTON_START, buttonStart);
    controller1.setButtonState(BUTTON_UP, buttonUp);
    controller1.setButtonState(BUTTON_DOWN, buttonDown);
    controller1.setButtonState(BUTTON_LEFT, buttonLeft);
    controller1.setButtonState(BUTTON_RIGHT, buttonRight);

    smbEngine->update();
    if (_doRendering) smbEngine->render(renderBuffer);
}

void renderFrame()
{
    SDL_UpdateTexture(texture, NULL, renderBuffer, sizeof(uint32_t) * RENDER_WIDTH);

    SDL_RenderClear(renderer);

    // Render the screen
    SDL_RenderSetLogicalSize(renderer, RENDER_WIDTH, RENDER_HEIGHT);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
}

size_t getStateSize()
{
  return smbEngine->getStateSize();
}

void saveState(jaffarCommon::serializer::Base& s)
{
  smbEngine->saveState(s);
}

void loadState(jaffarCommon::deserializer::Base& d)
{
 smbEngine->loadState(d);
}