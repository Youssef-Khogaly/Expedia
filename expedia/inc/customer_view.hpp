/*
 * customer_view.hpp
 *
 *  Created on: Mar 1, 2025
 *      Author: khogaly
 */

#ifndef CUSTOMER_VIEW_HPP_
#define CUSTOMER_VIEW_HPP_
#include <string>
#include <cassert>
#include <iostream>
#include <CustomerManager.hpp>
#include <utility.hpp>
#include <limits>
#include <vector>
namespace
{
  using std::vector;
  using std::string;
  class customer_view
  {

  private:
    CustomerManager &Cust_mgr;
    const customer &cust;
    ItenaryReservation CurrentItinerary;
    void
    ReadFlightRequest (FlightRequst &FlightReq_out) const
    {
      string str;
      int temp;
      str.reserve (32);
      str = ReadInput ("From:", "");
      FlightReq_out.SetFrom (str);
      str = ReadInput ("To:", "");
      FlightReq_out.SetTo (str);
      str = ReadInput ("Date From:", "");
      FlightReq_out.SetDateFrom (str);

      str = ReadInput ("Date To:", "");
      FlightReq_out.SetDateTo (str);

      temp = ReadInput (0, std::numeric_limits<int>::max (),
			"number of adults:", "");
      FlightReq_out.SetAdults (temp);
      temp = ReadInput (0, std::numeric_limits<int>::max (),
			"number of children:", "");
      FlightReq_out.SetChildern (temp);
      temp = ReadInput (0, std::numeric_limits<int>::max (),
			"number of infants:", "");
      FlightReq_out.SetInfants (temp);

    }

    void
    ReadHotelRequest (HotelRequest &HotelReq_out)
    {
      string str;
      int temp;
      str.reserve (32);
      str = ReadInput ("Country:", "");
      HotelReq_out.SetCountry (str);
      str = ReadInput ("City:", "");
      HotelReq_out.SetCity (str);
      str = ReadInput ("Date From:", "");
      HotelReq_out.SetDateFrom (str);
      str = ReadInput ("Date To:", "");
      HotelReq_out.SetDateTo (str);
      temp = ReadInput (0, std::numeric_limits<int>::max (),
			"number of adults:", "");
      HotelReq_out.SetAdults (temp);
      temp = ReadInput (0, std::numeric_limits<int>::max (),
			"number of children:", "");
      HotelReq_out.SetChildren (temp);
      temp = ReadInput (0, std::numeric_limits<int>::max (), "number of rooms:",
			"");
      HotelReq_out.SetRooms (temp);
    }
    int
    ChoseFlight (const vector<Flight> &Flights)
    {
      if (0 == Flights.size ())
	{
	  cout << "No avaliable flights for your request\n";
	  return -1;
	}
      int i = 1;
      for (const Flight &flight : Flights)
	{

	  cout << i++ << "-" << flight.ToStr () << "\n";
	}

      return (ReadInput (size_t (1), Flights.size (), "chose a flight:",
			 "invalid number\n") - 1);
    }
    int
    choseHotel (const vector<HotelRoom> &rooms)
    {
      if (0 == rooms.size ())
	{
	  cout << "No avaliable rooms for your request\n";
	  return -1;
	}
      int i = 1;
      for (const HotelRoom &room : rooms)
	{
	  cout << i++ << "-" << room.ToStr () << "\n";
	}
      return (ReadInput (size_t (1), rooms.size (), "chose hotel room",
			 "invalid number try again:") - 1);
    }
    void
    AddHotel ()
    {
      HotelRequest hotelReq;
      ReadHotelRequest (hotelReq);
      vector<HotelRoom> rooms = Cust_mgr.FindHotel (hotelReq);
      int roomNum = choseHotel (rooms);
      if (roomNum == -1)
	return;
      HotelRoom &UsrRoom = rooms.at (roomNum);
      HotelReservation reservation (UsrRoom, hotelReq);
      CurrentItinerary.AddReservation (reservation);
    }
    void
    AddFlight ()
    {
      FlightRequst flightReq;
      ReadFlightRequest (flightReq);
      vector<Flight> flights = Cust_mgr.FindFlight (flightReq);
      int flightNum = ChoseFlight (flights);
      if (flightNum == -1)
	return;
      Flight &usrFlight = flights.at (flightNum);
      FlightReservation reservation (usrFlight, flightReq);
      CurrentItinerary.AddReservation (reservation);
    }
    void
    PayItinerary ()
    {
      vector<string> PayChoices = Cust_mgr.GetPaymentCardsChoices ();
      for (int i = 0; i < PayChoices.size (); ++i)
	{
	  cout << i + 1 << "-" << PayChoices[i] << "\n";
	}
      int paymentChoice = ReadInput (size_t (1), PayChoices.size (),
				     "choice payment:",
				     "invalid choice try agian:") - 1;
      PaymentCard *card = Cust_mgr.GetCustomerCards ().at (paymentChoice);

      PaymentMethod method = static_cast<PaymentMethod> (ReadInput (
	  1, 3, "chose payment method \n1-paypal\n2-strip\n3-Square\n:",
	  "invalid method enter number in range") - 1);

      bool stat = Cust_mgr.MakeReservation (CurrentItinerary, *card, method);

      if (stat)
	{
	  cout << "Itinerary reservation confirmed\n";
	}

      else
	cout << "Itinerary reservation failed\n";

      return;
    }
  public:
    customer_view () = delete;
    customer_view (const customer_view&) = delete;
    customer_view&
    operator= (const customer_view&) = delete;
    explicit
    customer_view (CustomerManager &Cust_mgr) :
	Cust_mgr (Cust_mgr), cust (Cust_mgr.GetCustomer ()), CurrentItinerary ()
    {
    }
    void
    ViewProfile () const
    {
      cout << cust.ToStr () << endl << endl;
    }

    void
    MakeItenary ()
    {
      vector<string> Menu
	{
	  { "Add Flight" },
	  { "Add Hotel" },
	  { "Done" },
	  { "Cancel" } };
      int choice;
      while (true)
	{
	  choice = ShowMenu (Menu);
	  switch (choice)
	    {
	    case 1:
	      AddFlight ();
	      break;
	    case 2:
	      AddHotel ();
	      break;
	    case 3:
	      PayItinerary ();
	      CurrentItinerary.ClearList ();
	      return;
	    case 4:
	      CurrentItinerary.ClearList ();
	      return;
	    }
	}
    }
    void
    ListItienrary () const
    {
      vector<string> itinerary = Cust_mgr.GetItenaries ();
      if (itinerary.size () == 0)
	cout << "No itinerary reserved\n";
      else
	{
	  cout << "size = " << itinerary.size () << endl;
	  for (const auto &str : itinerary)
	    cout << str << "\n";
	}

    }
    void
    Display ()
    {
      cout << "\n\nHello " << cust.GetName () << " | User View\n";
      int choice = 0;
      vector<string> Menu
	{
	  { "View Profile" },
	  { "Make itinerary" },
	  { "List itinerary" },
	  { "Logout" } };
      while (true)
	{
	  choice = ShowMenu (Menu);
	  if (choice == 1)
	    ViewProfile ();
	  else if (choice == 2)
	    MakeItenary ();
	  else if (choice == 3)
	    {
	      ListItienrary ();

	    }
	  else
	    {

	      break;
	    }
	}

    }

  };
}

#endif /* CUSTOMER_VIEW_HPP_ */
