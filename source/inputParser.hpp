#pragma once

// Base controller class
// by eien86

#include <cstdint>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/json.hpp>
#include <string>
#include <sstream>

namespace jaffar
{
  
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

class InputParser
{
public:

  InputParser(const nlohmann::json &config)
  {
    
  }

  inline input_t parseInputString(const std::string &inputString) const
  {
    // Storage for the input
    input_t input;

    // Converting input into a stream for parsing
    std::istringstream ss(inputString);

    // Start separator
    if (ss.get() != '|') reportBadInputString(inputString);

    // Parsing console inputs
    parseConsoleInputs(input, ss, inputString);
    
    // Parsing controller 1 inputs
    parseControllerInputs(input, ss, inputString);

    // End separator
    if (ss.get() != '|') reportBadInputString(inputString);

    // If its not the end of the stream, then extra values remain and its invalid
    ss.get();
    if (ss.eof() == false) reportBadInputString(inputString);

    // Returning input
    return input;
  };

  private:

  static inline void reportBadInputString(const std::string &inputString)
  {
    JAFFAR_THROW_LOGIC("Could not decode input string: '%s'\n", inputString.c_str());
  }

  static bool parseJoyPadInput(input_t& input, std::istringstream& ss, const std::string& inputString)
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
    if (c != '.' && c != 'U') reportBadInputString(inputString);
    if (c == 'U') input.buttonUp = true;

    // Down
    c = ss.get();
    if (c != '.' && c != 'D') reportBadInputString(inputString);
    if (c == 'D') input.buttonDown = true;

    // Left
    c = ss.get();
    if (c != '.' && c != 'L') reportBadInputString(inputString);
    if (c == 'L') input.buttonLeft = true;

    // Right
    c = ss.get();
    if (c != '.' && c != 'R') reportBadInputString(inputString);
    if (c == 'R') input.buttonRight = true;

    // Start
    c = ss.get();
    if (c != '.' && c != 'S') reportBadInputString(inputString);
    if (c == 'S') input.buttonStart= true;

    // Select
    c = ss.get();
    if (c != '.' && c != 's') reportBadInputString(inputString);
    if (c == 's') input.buttonLeft = true;

    // B
    c = ss.get();
    if (c != '.' && c != 'B') reportBadInputString(inputString);
    if (c == 'B') input.buttonB = true;

    // A
    c = ss.get();
    if (c != '.' && c != 'A') reportBadInputString(inputString);
    if (c == 'A') input.buttonA = true;

    return true;
  }

  static void parseControllerInputs(input_t& input, std::istringstream& ss, const std::string& inputString)
  {
    // Controller separator
    if (ss.get() != '|') reportBadInputString(inputString);

    // Parsing joypad code
    parseJoyPadInput(input, ss, inputString);
  }

  static void parseConsoleInputs(input_t& input, std::istringstream& ss, const std::string& inputString)
  {
    // Currently read character
    char c;

    // Power trigger
    c = ss.get();
    if (c != '.' && c != 'P') reportBadInputString(inputString);
    if (c == 'P') input.power = true;
    if (c == '.') input.power = false;

    // Reset trigger
    c = ss.get();
    if (c != '.' && c != 'r') reportBadInputString(inputString);
    if (c == 'r') input.reset = true;
    if (c == '.') input.reset = false;
  }

}; // class inputParser

} // namespace jaffar