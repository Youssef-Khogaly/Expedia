/*
 * user.hpp
 *
 *  Created on: Feb 27, 2025
 *      Author: khogaly
 */

#ifndef USER_HPP_
#define USER_HPP_
#include <vector>
#include <string>
#include <iostream>
#include <Reservation_common.hpp>
#include <PaymentCards.hpp>
#include <cassert>
namespace
{
  using std::string;
  using std::vector;
  enum class UserType
  {
    CustomerUser = 0, AdminUser = 1
  };
  class user
  {

  protected:
    string userName;
    string password;
    string email;
    string name;
    UserType type;
  public:
    user () :
	type (UserType::CustomerUser)
    {
    }
    user (const string &userName, const string &password, const string &email,
	  const string &name, const UserType &type) :
	userName (userName), password (password), email (email), name (name), type (
	    type)
    {

    }
    //getters
    const string&
    GetUserName (void) const
    {
      return userName;
    }
    const string&
    GetPassword (void) const
    {
      return password;
    }
    const string&
    GetEmail (void) const
    {
      return email;
    }
    const string&
    GetName (void) const
    {
      return name;
    }
    UserType
    GetUserType (void) const
    {
      return type;
    }
    //setters
    user&
    SetUserName (const string &UserName)
    {
      userName = UserName;
      return *this;
    }
    user&
    SetPassword (const string &pass)
    {
      password = pass;
      return *this;
    }
    user&
    SetEmail (const string &EMAIL)
    {
      email = EMAIL;
      return *this;
    }
    user&
    SetName (const string &NAME)
    {
      name = NAME;
      return *this;
    }
    user&
    SetUserType (const UserType &TYPE)
    {
      type = TYPE;
      return *this;
    }

    virtual string
    ToStr () const
    {

      ostringstream oss;

      oss << "Name: " << GetName ();
      if (GetUserType () == UserType::AdminUser)
	oss << " | Admin" << endl;
      else
	oss << endl;
      oss << "Email:" << GetEmail () << endl;
      oss << "User name:" << GetUserName ();
      return oss.str ();
    }
    virtual user*
    clone () const
    {
      return new user (*this);
    }
    virtual
    ~user () = default;
  };

  class customer : public user
  {
  protected:
    ReservationSet reservationSet;
    vector<PaymentCard*> Cards;
  public:
    customer () = default;
    customer (const customer &_other) :
	reservationSet (_other.reservationSet)
    {
      for (auto &card : _other.Cards)
	{
	  this->AddCard (*card);
	}
    }
    customer (customer &&_other) :
	reservationSet (std::move (_other.reservationSet)), Cards (_other.Cards)
    {

      // nulling other ptr
      for (auto &pCard : _other.Cards)
	pCard = nullptr;
    }
    customer&
    operator = (const customer &_other) = delete;
    customer&
    operator = (customer &&_other)
    {
      auto &me = *this;
      if (this == &_other)
	return *this;

      // free my resources
      reservationSet.~ItenaryReservation ();
      reservationSet = std::move (_other.reservationSet);
      Cards = _other.Cards;

      // nulling other ptrs
      for (auto &pCard : _other.Cards)
	pCard = nullptr;
      return me;
    }
    const vector<PaymentCard*>&
    GetCards () const
    {
      return Cards;
    }
    const ReservationSet&
    GetReservations () const
    {
      return reservationSet;
    }
    void
    AddCard (const PaymentCard &Card)
    {
      Cards.push_back (Card.clone ());
    }
    void
    DeleteCard (int CardIdx)
    {
      assert(CardIdx >= 0);
      vector<PaymentCard*>::iterator it = Cards.begin () + CardIdx;
      delete *it;
      *it = nullptr;
      Cards.erase (it);
    }
    void
    AddReservation (const Reservation &res)
    {
      Reservation *cpy = res.clone ();
      reservationSet.AddReservation (*cpy);
    }
    void
    CancelReservation (int ReservationIdx)
    {
      reservationSet.CancelReservation (ReservationIdx);
    }
    // cancel all reservations
    void
    CancelReservation ()
    {
      reservationSet.~ItenaryReservation ();
    }
    virtual user*
    clone () const override
    {
      return new customer (*this);
    }
    virtual
    ~customer ()
    {
      reservationSet.~ItenaryReservation ();
      for (auto &card : Cards)
	{
	  if (card != nullptr)
	    {
	      delete card;
	      card = nullptr;
	    }
	}
      Cards.clear ();
    }
  };

  class admin : public user
  {

  public:
    admin () :
	user ()
    {
      user::SetUserType (UserType::AdminUser);
    }
    virtual user*
    clone () const override
    {
      return new admin (*this);
    }
  };
}

#endif /* USER_HPP_ */
