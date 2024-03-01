#include "VirtualMachine.h"

// Constructor
VirtualMachine::VirtualMachine()
{
  reg = new unsigned char[7]();    // Initialize register array with 7 elements
  mem = new unsigned char[SIZE](); // Initialize memory array with a size defined by SIZE
  of = zf = cf = uf = false;       // Initialize flags to false
  counter = 0;                     // Initialize counter to 0
}

// Destructor
VirtualMachine::~VirtualMachine()
{
  delete[] reg; // Deallocate memory for the register array
  delete[] mem; // Deallocate memory for the memory array
  reg = mem = nullptr;
}

/********************************STRING HANDLING************************************/

// Trim leading and trailing spaces and commas from a string
string VirtualMachine::trim(string str)
{
  // Code for trimming left leading spaces and commas
  int i = 0;
  while (i < str.length())
  {
    if (str[0] == ' ' || str[0] == ',')
      str.erase(0, 1);
    i++;
  }

  // Code for trimming right trailing spaces and commas
  i = str.length() - 1;
  while (i >= 0)
  {
    if (str[str.length() - 1] == ' ' || str[str.length() - 1] == ',')
      str.erase(str.length() - 1, 1);
    i--;
  }
  return str;
}

// Execute instructions from a file
int VirtualMachine::execFile(string filename)
{
  string line;
  ifstream file;
  file.open(filename);
  if (file.fail())
  {
    cout << "Error opening file" << endl;
    return -1;
  }
  while (getline(file, line))
  {
    string opcode, operand1, operand2;
    string content;
    stringstream iss(line);
    iss >> opcode >> operand1 >> operand2;
    opcode = trim(opcode);
    operand1 = trim(operand1);
    operand2 = trim(operand2);
    cout << line << endl;
    processInstruction(opcode, operand1, operand2);
    content = getTemplate(++counter);
    cout << content << endl;
    writeFile(content, filename);
  }
  file.close();
  return 0;
}

// Process individual instruction based on opcode
void VirtualMachine::processInstruction(const string opcode, const string operand1, const string operand2)
{
  if (opcode == "IN")
  {
    processIN(operand1);
  }
  else if (opcode == "OUT")
  {
    processOUT(operand1);
  }
  else if (opcode == "MOV")
  {
    processMOV(operand1, operand2);
  }
  else if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL" || opcode == "DIV")
  {
    processArithmetic(opcode, operand1, operand2);
  }
  else if (opcode == "INC" || opcode == "DEC")
  {
    processIncDec(opcode, operand1);
  }
  else if (opcode == "ROL" || opcode == "ROR")
  {
    processRotate(opcode, operand1, operand2);
  }
  else if (opcode == "SHL" || opcode == "SHR")
  {
    processShift(opcode, operand1, operand2);
  }
  else if (opcode == "LOAD")
  {
    processLOAD(operand1, operand2);
  }
  else if (opcode == "STORE")
  {
    processSTORE(operand1, operand2);
  }
  else
  {
    cout << "Unknown opcode: " << opcode << endl;
    exit(0);
  }
}

/********************************OUTPUT*********************************************/

// Set flags based on result value and carry condition
void VirtualMachine::setFlag(const int value, bool carry = false)
{
  of = of || (value > 255);
  cf = cf || (carry == true && value > 255);
  zf = zf || (value == 0);
  uf = uf || (value < 0);
}

// Generate a template string for displaying the current state
string VirtualMachine::getTemplate(int counter)
{
  stringstream content;
  content << setw(8) << "Register: ";
  for (int i = 0; i < 7; i++)
  {
    content << setw(2) << setfill('0') << static_cast<int>(reg[i]);
    if (i < 6)
      content << " ";
  }
  content << '#' << endl;
  content << setw(8) << "Flags   : " << cf << " " << of << " " << uf << " " << zf << "#" << endl;
  content << left << setw(8) << "PC.     : " << counter << endl;
  content << endl << setw(8) << "Memory  :" << endl;
  for (int i = 0; i < SIZE; i++)
  {
    string value_str = to_string(static_cast<int>(mem[i]));

    if (value_str.length() < 2)
      value_str = "0" + value_str;

    content << setw(2) << setfill('0') << value_str << " ";
    if ((i + 1) % 8 == 0)
      content << endl;
  }
  content << "#" << endl;
  return content.str();
}

// Write the template content to a file
void VirtualMachine::writeFile(string content, string filename)
{
  ofstream file;
  filename = filename.substr(0, filename.length() - 4) + ".txt";
  file.open(filename);
  file << content;
  file.close();
}

/********************************PROCESS********************************************/

// Process IN instruction
void VirtualMachine::processIN(const string operand)
{
  string input;
  bool isNumber = false;
  int pos = stoi(operand.substr(1, 1));
  while (!isNumber)
  {
    cout << operand << ": " << flush;
    getline(cin, input);
    for (int i = 0; i < input.length(); i++)
    {
      isNumber = isdigit(input[i]);
      if (!isNumber)
      {
        cout << "Invalid input. Please enter a number." << endl << endl;
        break;
      }
    }
  }
  setFlag(stoi(input));
  reg[pos] = stoi(input);
}

// Process OUT instruction
void VirtualMachine::processOUT(const string operand)
{
  int pos = stoi(operand.substr(1, 1));
  cout << operand << ": " << static_cast<int>(reg[pos]) << endl;
}

// Process MOV instruction
void VirtualMachine::processMOV(const string operand1, const string operand2)
{
  int rdst = stoi(operand2.substr(1, 1));
  if (operand1[0] == 'R')
  {
    int rsrc = stoi(operand1.substr(1, 1));
    reg[rdst] = reg[rsrc];
  }
  else if (operand1[0] == '[' && operand1[operand1.length() - 1] == ']')
  {
    int rsrc = stoi(operand1.substr(2, 1));
    int pos = reg[rsrc];
    outofBound(pos);
    unsigned char *ptr = &mem[pos];
    reg[rdst] = *ptr;
  }
  else
  {
    unsigned char rsrc = stoi(operand1);
    reg[rdst] = rsrc;
  }
}

// Process arithmetic instructions (ADD, SUB, MUL, DIV)
int VirtualMachine::processArithmetic(const string opcode, const string operand1, const string operand2)
{
  int rdst = stoi(operand2.substr(1, 1));
  int rsrc;
  rsrc = (operand1[0] == 'R') ? stoi(operand1.substr(1, 1)) : stoi(operand1);
  int num = 0;
  if (opcode == "ADD")
  {
    num = reg[rdst] + ((operand1[0] == 'R') ? reg[rsrc] : rsrc);
    setFlag(num, true);
    reg[rdst] = num;
    return 0;
  }
  else if (opcode == "SUB")
  {
    num = reg[rdst] - ((operand1[0] == 'R') ? reg[rsrc] : rsrc);
  }
  else if (opcode == "MUL")
  {
    num = reg[rdst] * ((operand1[0] == 'R') ? reg[rsrc] : rsrc);
    setFlag(num, true);
    reg[rdst] = num;
    return 0;
  }
  else if (opcode == "DIV")
  {
    if ((operand1[0] == 'R' && reg[rsrc] == 0) || (operand1[0] != 'R' && rsrc == 0))
    {
      cout << "Division by zero error" << endl;
      exit(0);
    }
    num = reg[rdst] / ((operand1[0] == 'R') ? reg[rsrc] : rsrc);
  }
  setFlag(num);
  reg[rdst] = num;
  return 0;
}

// Process INC and DEC instructions
void VirtualMachine::processIncDec(const string opcode, const string operand)
{
  int num = 0;
  int rsrc = stoi(operand.substr(1, 1));
  if (opcode == "INC")
  {
    num = reg[rsrc] + 1;
    setFlag(num, true);
  }
  else if (opcode == "DEC")
  {
    num = reg[rsrc] - 1;
    setFlag(num);
  }
  reg[rsrc] = num;
}

// Process ROL and ROR instructions
void VirtualMachine::processRotate(const string opcode, const string operand1, const string operand2)
{
  int rsrc = stoi(operand1.substr(1, 1));
  int rotate = stoi(operand2) % 8;
  reg[rsrc] = (opcode == "ROL") ? (reg[rsrc] << rotate) | (reg[rsrc] >> (8 - rotate)) : (reg[rsrc] >> rotate) | (reg[rsrc] << (8 - rotate));
}

string VirtualMachine::charToBinaryString(unsigned char c)
{
  int binaryarray[8];
  // Convert the character to binary representation using powers of 2
  for (int i = 7; i >= 0; i--)
  {
    if (c - pow(2, i) >= 0)
    {
      // If the current power of 2 can be subtracted from the character,
      // set the corresponding bit to 1 and update the character
      binaryarray[7 - i] = 1;
      c -= pow(2, i);
    }
    else
    {
      // Otherwise, set the corresponding bit to 0
      binaryarray[7 - i] = 0;
    }
  }
  // Construct the binary string from the array
  string binaryString;
  for (int j = 0; j < 8; j++)
  {
    // Append each bit to the binary string
    binaryString += to_string(binaryarray[j]);
  }
  return binaryString;
}

unsigned char VirtualMachine::binaryStringToChar(const string &binaryString)
{
  unsigned char result = 0;
  // Convert the binary string to a character using powers of 2
  for (int i = 0; i < 8; ++i)
  {
    if (binaryString[i] == '1')
    {
      // If the current bit is 1, add the corresponding power of 2 to the result
      result += pow(2, 7 - i);
    }
  }
  return result;
}

// Process SHL and SHR instructions
void VirtualMachine::processShift(const string opcode, const string operand1, const string operand2)
{
  int rsrc = stoi(operand1.substr(1, 1));
  int shift = stoi(operand2);
  string binary = charToBinaryString(reg[rsrc]);
  shift = (shift >= 8) ? 8 : shift; // any shift above 8, consider as 8 only
  binary = (opcode == "SHL") ? binary.substr(shift) + string(shift, '0') : string(shift, '0') + binary.substr(0, 8 - shift);
  reg[rsrc] = binaryStringToChar(binary);
}

// Process LOAD instruction
void VirtualMachine::processLOAD(const string operand1, const string operand2)
{
  int rsrc = stoi(operand1.substr(1, 1));
  if (operand2[0] == '[' && operand2[operand2.length() - 1] == ']')
  {
    int rdst = stoi(operand2.substr(2, 1));
    unsigned char *ptr = &reg[rdst];
    outofBound(*ptr);
    reg[rsrc] = mem[*ptr];
  }
  else
  {
    int rdst = stoi(operand2);
    outofBound(rdst);
    reg[rsrc] = mem[rdst];
  }
}

// Process STORE instruction
void VirtualMachine::processSTORE(const string operand1, const string operand2)
{
  int rsrc = stoi(operand1.substr(1, 1));
  if (operand2[0] == '[' && operand2[operand2.length() - 1] == ']')
  {
    int rdst = stoi(operand2.substr(2, 1));
    int pos = reg[rdst];
    outofBound(pos);
    unsigned char *ptr = &reg[rsrc];
    mem[pos] = *ptr;
  }
  else
  {
    int rdst = stoi(operand2);
    outofBound(rdst);
    mem[rdst] = reg[rsrc];
  }
}

// Check if the memory access is within bounds
void VirtualMachine::outofBound(const int num)
{
  if (num >= 64)
  {
    cout << "Memory: Out of Bound" << endl;
    exit(0);
  }
}
