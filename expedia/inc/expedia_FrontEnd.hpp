/*
 * expedia_FrontEnd.hpp
 *
 *  Created on: Mar 2, 2025
 *      Author: khogaly
 */

#ifndef EXPEDIA_FRONTEND_HPP_
#define EXPEDIA_FRONTEND_HPP_

#include <vector>
#include <string>
#include <utility.hpp>
#include <CustomerManager.hpp>
#include <customer_view.hpp>
#include <UserManager.hpp>
#include <ExpediaBackEnd.hpp>
namespace
{
  using std::vector;
  using std::string;
  class ExpediaFrontEnd
  {

  private:

    UserManager usrMgr;
    BackEnd Exp_BackEnd;
    CustomerManager *CustMgr;
    void
    LoadDataBase ()
    {
      usrMgr.LoadDataBase ();

    }
    void
    AccessSystem ()
    {
      vector<string> Menu
	{
	  { "Signup" },
	  { "Login" } };
      bool break_ = true;
      bool stat = false;
      int choice;

      while (break_)
	{
	  choice = ShowMenu (Menu);
	  if (1 == choice)
	    {
	      stat = usrMgr.DoSignUp ();
	      if (stat == true)
		cout << "Signed up Successfully\n";
	      else
		cout << "Signed up failed try again\n";
	    }
	  else if (2 == choice)
	    {
	      stat = usrMgr.DoLogin ();

	      if (stat)
		{
		  cout << "logged in Successfully\n";
		  break_ = false;
		}
	      else
		{
		  cout << "Invalid User name or password\n";
		}
	    }
	}

    }
  public:
    ExpediaFrontEnd () :
	usrMgr (UserManager
	  { }), Exp_BackEnd (BackEnd
	  { }), CustMgr (nullptr)
    {

    }
    ExpediaFrontEnd (const ExpediaFrontEnd&) = delete;
    ExpediaFrontEnd&
    operator = (const ExpediaFrontEnd&) = delete;
    ~ExpediaFrontEnd ()
    {
      if (CustMgr)
	delete CustMgr;

      CustMgr = nullptr;
    }
    void
    Run ()
    {
      LoadDataBase ();

      while (1)
	{
	  AccessSystem ();
	  if (usrMgr.GetCurrentUser ()->GetUserType ()
	      == UserType::CustomerUser)
	    {
	      CustMgr = new CustomerManager (usrMgr.GetCurrentUser (),
					     Exp_BackEnd);
	      customer_view view (*CustMgr);
	      view.Display (); // forever loop till user logout
	    }
	  else
	    assert(0);

	}

    }
  }
  ;
}
;

#endif /* EXPEDIA_FRONTEND_HPP_ */
