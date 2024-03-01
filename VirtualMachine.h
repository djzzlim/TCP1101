#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

// Class representing a simple virtual machine
class VirtualMachine
{
private:
  const int SIZE = 64;      // Size of memory
  unsigned char *reg, *mem; // Registers and memory
  bool of, zf, cf, uf;      // Flags for overflow, zero, carry, and underflow
  int counter;              // Program counter

public:
  // Constructor
  VirtualMachine();

  // Destructor
  ~VirtualMachine();

  // Function to trim leading and trailing whitespaces
  string trim(string str);

  // Execute instructions from a file
  int execFile(string filename);

  // Get the instruction template at a specific counter
  string getTemplate(int counter);

  // Set flags based on the result of an operation
  void setFlag(const int value, bool addition);

  // Process an instruction
  void processInstruction(const string opcode, const string operand1, const string operand2);

  // Write content to a file
  void writeFile(string content, string filename);

  // Process input instruction
  void processIN(const string operand);

  // Process output instruction
  void processOUT(const string operand);

  // Process move instruction
  void processMOV(const string operand1, const string operand2);

  // Process arithmetic instruction
  int processArithmetic(const string opcode, const string operand1, const string operand2);

  // Process increment/decrement instruction
  void processIncDec(const string opcode, const string operand);

  // Convert character to binary string
  string charToBinaryString(unsigned char c);

  // Convert binary string to character
  unsigned char binaryStringToChar(const string &binaryString);

  // Process rotate instruction
  void processRotate(const string operation, const string operand1, const string operand2);

  // Process shift instruction
  void processShift(const string opcode, const string operand1, const string operand2);

  // Process load instruction
  void processLOAD(const string operand1, const string operand2);

  // Process store instruction
  void processSTORE(const string operand1, const string operand2);

  // Handle out-of-bounds memory access
  void outofBound(const int num);
};

#endif
