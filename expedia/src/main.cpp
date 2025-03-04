//============================================================================
// Name        : expedia.cpp
// Author      : Youssef
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <expedia_FrontEnd.hpp>

/* For debugging */
extern "C" void
__cxa_pure_virtual ()
{
  cout << "Pure Virtual called !! HERE \n\n";
  throw std::runtime_error ("pure virtual function called");
}
int
main ()
{
  ExpediaFrontEnd exp;
  exp.Run ();
  return 0;
}

