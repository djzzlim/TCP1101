/********************************************
Course: TCP1101 Programming Fundamentals
Assignment: Assembly Language Interpreter
Session : T2310

Group Leader:
ID & Name 1: 1211108893, Lim Kar Joon
Phone,Email: 01136118955, djzzlim@gmail.com

ID & Name 2: 1211107112, Lam Rong Yi
Phone,Email: 0176952983, lamrongyi983@gmail.com

ID & Name 3: 1211106712, Willie Teoh Chin Wei
Phone,Email: 0102020873, willieteoh11@gmail.com

ID & Name 4: 1211103413, Emil Hakim Bin Zainuddin
Phone,Email: 0126248645, ehakim319@gmail.com
********************************************/
#include "VirtualMachine.h"

int main()
{
  cout << "Lim Kar Joon, Lam Rong Yi, Willie Teoh Chin Wei, Emil Hakim Bin Zainuddin\n\n\n";

  VirtualMachine vm;
  vm.execFile("file.asm");

  return 0;
}