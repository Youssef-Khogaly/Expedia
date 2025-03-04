/*
 * UserManager.hpp
 *
 *  Created on: Feb 27, 2025
 *      Author: khogaly
 */

#ifndef USERMANAGER_HPP_
#define USERMANAGER_HPP_

#include <string>
#include <map>
#include <user.hpp>
#include <utility.hpp>
namespace
{
  using std::string;
  using std::map;
  class UserManager
  {
  private:
    map<string, user*> userName_Obj_map;
    user *CurrentUser;

    void
    FreeLoadedData ()
    {
      for (auto& [name, obj] : userName_Obj_map)
	{
	  if (obj != nullptr)
	    {
	      delete obj;
	      obj = nullptr;
	    }
	}
    }
  public:
    UserManager () :
	userName_Obj_map (map<string, user*>
	  { }), CurrentUser (nullptr)
    {

    }
    UserManager (const UserManager&) = delete;
    UserManager&
    operator = (const UserManager&) = delete;
    void
    LoadDataBase ()
    {

      FreeLoadedData ();

      // Some "Dummy Data" for simplicity
      admin *admin_user = new admin ();
      admin_user->SetUserName ("yossef");
      admin_user->SetPassword ("111");
      admin_user->SetEmail ("yossef@gmail.com");
      admin_user->SetName ("Yossef Ibrahim");
      userName_Obj_map[admin_user->GetUserName ()] = admin_user;

      customer *customrUser = new customer ();
      customrUser->SetUserName ("ali");
      customrUser->SetPassword ("222");
      customrUser->SetEmail ("ali@gmail.com");
      customrUser->SetName ("ali");

      DebitCard *debitCard = new DebitCard ();
      debitCard->SetName (customrUser->GetName ());
      debitCard->SetCardNumber ("11-22-33-44");
      debitCard->Set_ExpDate ("11/20");
      debitCard->Set_CCV (111);
      debitCard->SetBillingAddress ("111 hello st, BC, Canada");
      customrUser->AddCard (*debitCard);

      CreditCard *creditCard = new CreditCard ();
      creditCard->SetName (customrUser->GetName ());
      creditCard->SetCardNumber ("22-11-03-44");
      creditCard->Set_ExpDate ("11/25");
      creditCard->Set_CCV (1117);
      customrUser->AddCard (*creditCard);
      userName_Obj_map[customrUser->GetUserName ()] =
	  static_cast<user*> (customrUser);
    }

    bool
    DoSignUp ()
    {
      bool ret = false;
      customer *newusr = new customer ();
	{
	}
      ;
      newusr->SetName (ReadInput ("Enter name: ", "invalid name\n"));
      newusr->SetUserName (ReadInput ("Enter User name: ", "invalid name\n"));
      while (userName_Obj_map.count (newusr->GetUserName ()) != 0)
	{
	  cout << "invalid user name try another one\n";
	  newusr->SetUserName (ReadInput ("Enter name: ", "invalid name\n"));
	}

      newusr->SetEmail (ReadInput ("Enter email: ", "invalid password\n"));
      newusr->SetPassword (
	  ReadInput ("Enter password: ", "invalid password\n"));
      newusr->SetUserType (UserType::CustomerUser);
      userName_Obj_map.insert (
	{ newusr->GetUserName (), newusr });

      ret = true;
      return ret;
    }
    bool
    DoLogin ()
    {
      string userName;
      string Pass;
      userName = ReadInput (string
	{ "Enter user name:" },
			    string
			      { "" });
      Pass = ReadInput (string
	{ "Enter Password:" },
			string
			  { "" });
      bool ret = false;
      try
	{
	  if (Pass == userName_Obj_map.at (userName)->GetPassword ())
	    {
	      ret = true;
	      CurrentUser = userName_Obj_map.at (userName);
	    }
	}
      catch (std::out_of_range &err)
	{
	  // invalid user name;
	  ret = false;
	}

      return ret;
    }
    user*
    GetCurrentUser ()
    {
      return CurrentUser;
    }
  };
}

#endif /* USERMANAGER_HPP_ */
