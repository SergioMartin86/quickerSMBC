#pragma once

// Base controller class
// by eien86

#include <cstdint>
#include <string>
#include <sstream>

namespace smbc
{

class Controller
{
public:

  struct input_t
  {
    bool buttonA = false;
    bool buttonB = false;
    bool buttonSelect = false;
    bool buttonStart = false;
    bool buttonUp = false;
    bool buttonDown = false;
    bool buttonLeft = false;
    bool buttonRight = false;
    bool power = false;
    bool reset = false;
  };

  inline bool parseInputString(const std::string& input)
  {
    // Parse valid flag
    bool isValid = true;

    // Converting input into a stream for parsing
    std::istringstream ss(input);

    // Start separator
    if (ss.get() != '|') isValid = false;

    // Parsing console inputs
    isValid &= parseConsoleInputs(_input.reset, _input.power, ss);

    // Parsing controller 1 inputs
    isValid &= parseControllerInputs(_input, ss);

    // End separator
    if (ss.get() != '|') isValid = false;

    // If its not the end of the stream, then extra values remain and its invalid
    ss.get();
    if (ss.eof() == false) isValid = false;

    // Returning valid flag
    return isValid;
  };

  inline input_t getInput() { return _input; }

  private:

  static bool parseJoyPadInput(input_t& input, std::istringstream& ss)
  {
    // Currently read character
    char c;

    // Cleaning code
    input.buttonA = false;
    input.buttonB = false;
    input.buttonSelect = false;
    input.buttonStart = false;
    input.buttonUp = false;
    input.buttonDown = false;
    input.buttonLeft = false;
    input.buttonRight = false;

    // Up
    c = ss.get();
    if (c != '.' && c != 'U') return false;
    if (c == 'U') input.buttonUp = true;

    // Down
    c = ss.get();
    if (c != '.' && c != 'D') return false;
    if (c == 'D') input.buttonDown = true;

    // Left
    c = ss.get();
    if (c != '.' && c != 'L') return false;
    if (c == 'L') input.buttonLeft = true;

    // Right
    c = ss.get();
    if (c != '.' && c != 'R') return false;
    if (c == 'R') input.buttonRight = true;

    // Start
    c = ss.get();
    if (c != '.' && c != 'S') return false;
    if (c == 'S') input.buttonStart= true;

    // Select
    c = ss.get();
    if (c != '.' && c != 's') return false;
    if (c == 's') input.buttonLeft = true;

    // B
    c = ss.get();
    if (c != '.' && c != 'B') return false;
    if (c == 'B') input.buttonB = true;

    // A
    c = ss.get();
    if (c != '.' && c != 'A') return false;
    if (c == 'A') input.buttonA = true;

    return true;
  }

  static bool parseControllerInputs(input_t& input, std::istringstream& ss)
  {
    // Parse valid flag
    bool isValid = true; 
 
    // Controller separator
    if (ss.get() != '|') isValid = false;

    // Parsing joypad code
    isValid &= parseJoyPadInput(input, ss);

    // Return valid flag
    return isValid;
  }

  static bool parseConsoleInputs(bool& reset, bool& power, std::istringstream& ss)
  {
    // Parse valid flag
    bool isValid = true; 

    // Currently read character
    char c;

    // Power trigger
    c = ss.get();
    if (c != '.' && c != 'P') isValid = false;
    if (c == 'P') power = true;
    if (c == '.') power = false;

    // Reset trigger
    c = ss.get();
    if (c != '.' && c != 'r') isValid = false;
    if (c == 'r') reset = true;
    if (c == '.') reset = false;

    // Return valid flag
    return isValid;
  }

  input_t _input;
}; // class Controller

} // namespace quickNES