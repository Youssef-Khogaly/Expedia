/*
 * CustomerManager.hpp
 *
 *  Created on: Mar 1, 2025
 *      Author: khogaly
 */

#ifndef CUSTOMERMANAGER_HPP_
#define CUSTOMERMANAGER_HPP_
#include <vector>
#include <user.hpp>
#include <ExpediaBackEnd.hpp>
#include <Flights.hpp>
#include <Hotels.hpp>
namespace
{
  using std::string;
  using std::vector;
  using std::string;
  class CustomerManager
  {
  private:
    customer *_customer;
    BackEnd &backend;
  public:
    CustomerManager () = delete;

    CustomerManager (user *_usr, BackEnd &backend) :
	_customer (dynamic_cast<customer*> (_usr)), backend (backend)
    {
      if (nullptr == _customer)
	{
	  cout << "Error null ptr";
	  assert(0);
	}
    }
    // copy constructor
    CustomerManager (const CustomerManager&) = delete;
    // copy assignment
    CustomerManager&
    operator= (const CustomerManager&) = delete;

    vector<Flight>
    FindFlight (const FlightRequst &Req) const
    {
      return backend.FindFlight (Req);
    }
    vector<HotelRoom>
    FindHotel (const HotelRequest &Req) const
    {
      return backend.FindHotel (Req);
    }
    vector<string>
    GetPaymentCardsChoices () const
    {
      const vector<PaymentCard*> &cards = _customer->GetCards ();
      vector<string> cardsInfo
	{ };
      cardsInfo.reserve (cards.size ());

      for (PaymentCard *card : cards)
	{
	  cardsInfo.push_back (card->ToStr ());
	}
      return cardsInfo;
    }

    bool
    MakeReservation (const Reservation &res, PaymentCard &payment_card,
		     PaymentMethod method)
    {
      bool paid = backend.ChargeCost (res.getTotalCost (), payment_card,
				      method);
      bool confirmed = false;
      bool uncharge = true;
      if (paid)
	{
	  cout << "Money Withdraw Successfully\n";
	  confirmed = backend.ConfirmReservation (res);
	  if (confirmed)
	    {
	      cout << "Reservation confirmed\n";
	      _customer->AddReservation (res);
	    }
	  else
	    {
	      cout << "Failed to confirm some of your items\n";
	      cout << "Canceling payment and whatever reserved\n";
	      bool uncharge = backend.UnChargeCost (res.getTotalCost (),
						    payment_card, method);
	      if (!uncharge)
		cout << "Problem in returning back your money\n";
	    }

	}
      else
	{
	  cout << "Failed to withdtaw Money for the reservation\n";
	}
      return (paid && confirmed);
    }
    vector<string>
    GetItenaries () const
    {
      vector<string> ret
	{ };
      const ReservationSet &Objs = _customer->GetReservations ();

      for (const auto &res : Objs.GetReservations ())
	{
	  ret.push_back (res->ToStr ());
	}
      return ret;
    }
    const vector<PaymentCard*>&
    GetCustomerCards () const
    {
      return _customer->GetCards ();
    }
    void
    AddReservationToList (const Reservation &res)
    {
      _customer->AddReservation (res);
    }
    const ReservationSet&
    GetReervationSet () const
    {
      return _customer->GetReservations ();
    }
    const customer&
    GetCustomer ()
    {
      return *_customer;
    }
  };
}

#endif /* CUSTOMERMANAGER_HPP_ */
