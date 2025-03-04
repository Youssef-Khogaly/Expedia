/*
 * utility.hpp
 *
 *  Created on: Feb 28, 2025
 *      Author: khogaly
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include <iostream>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <limits>
#include <algorithm>
namespace
{
  using std::vector;
  using std::initializer_list;
  using std::string;
  using std::cin;
  using std::cout;
  using std::endl;
  using std::find;
  using std::numeric_limits;
  using std::streamsize;
  template<typename T>
    T
    ReadInput (const initializer_list<T> &Choices, const string &InputMessage,
	       const string &ErrorMessage)
    {
      T input
	{ };
      cout << InputMessage;
      cin >> input;
      auto &it = find (Choices.begin (), Choices.end (), input);
      while (cin.fail () || it == Choices.end ())
	{
	  cin.clear ();
	  cin.ignore (numeric_limits<streamsize>::max (), '\n');
	  cout << ErrorMessage;
	  cin >> input;
	  it = find (Choices.begin (), Choices.end (), input);
	}

      return input;
    }

  template<typename T>
    T
    ReadInput (const initializer_list<T> &Choices, const T &lowest,
	       const T &Highest, const string &InputMessage,
	       const string &ErrorMessage)
    {
      T input
	{ };
      cout << InputMessage;
      cin >> input;
      auto it = find (Choices.begin (), Choices.end (), input);
      bool inRange = (input >= lowest && input <= Highest);
      while (cin.fail () || (it == Choices.end () && !inRange))
	{
	  cin.clear ();
	  cin.ignore (numeric_limits<streamsize>::max (), '\n');
	  cout << ErrorMessage;
	  cin >> input;
	  it = find (Choices.begin (), Choices.end (), input);
	  inRange = (input >= lowest && input <= Highest);
	}

      return input;
    }

  template<typename T>
    T
    ReadInput (const T &lowest, const T &Highest, const string &InputMessage,
	       const string &ErrorMessage)
    {
      T input
	{ };
      cout << InputMessage;
      cin >> input;
      bool inRange = (input >= lowest && input <= Highest);
      while (cin.fail () || !inRange)
	{
	  cin.clear ();
	  cin.ignore (numeric_limits<streamsize>::max (), '\n');
	  cout << ErrorMessage;
	  cin >> input;
	  inRange = (input >= lowest && input <= Highest);
	}
      cout << "\n";
      return input;
    }

  /*this function read input spaces*/
  template<typename T>
    string
    ReadInput (const initializer_list<string> &Choices,
	       const string &InputMessage, const string &ErrorMessage)
    {
      string input
	{ };
      cout << InputMessage;
      std::getline (cin, input, '\n');
      auto &it = find (Choices.begin (), Choices.end (), input);
      while (cin.fail () || it == Choices.end ())
	{
	  cin.clear ();
	  cin.ignore (numeric_limits<streamsize>::max (), '\n');
	  cout << ErrorMessage;
	  std::getline (cin, input, '\n');
	  it = find (Choices.begin (), Choices.end (), input);
	}

      return input;
    }

  string
  ReadInput (const string &InputMessage, const string &ErrorMessage)
  {
    string input
      { "" };
    cout << InputMessage << std::flush;

    while (input.size () == 0)
      std::getline (cin, input, '\n');

    while (cin.fail ())
      {
	cin.clear ();
	cin.ignore (numeric_limits<streamsize>::max (), '\n');
	if (ErrorMessage.size () > 0)
	  cout << ErrorMessage;
	std::getline (cin, input, '\n');
      }
    return input;
  }

  int
  ShowMenu (const vector<string> &Menu)
  {
    int i = 1;
    for (const auto &str : Menu)
      {
	cout << i++ << "-" << str << "\n";
      }
    return ReadInput (size_t (1), Menu.size (), ":",
		      "Invalid Number Try again\n");
  }
}

#endif /* UTILITY_HPP_ */
