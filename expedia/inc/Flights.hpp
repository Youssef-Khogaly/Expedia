/*
 * A_flights_api.hpp
 *
 *  Created on: Feb 20, 2025
 *      Author: khogaly
 */

#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <expedia_flights_api.hpp>
#include <Reservation_common.hpp>
#include <string>
#include <cassert>

class Flight
{
private:
  std::string AirLineName;
  std::string DateTimeFrom;
  std::string DateTimeTo;

  double totalCost
    { 0.0 };
public:
  Flight (const string &AirLineName, const string &DateTimeFrom,
	  const string &DateTimeTo, double cost) :
      AirLineName (AirLineName), DateTimeFrom (DateTimeFrom), DateTimeTo (
	  DateTimeTo), totalCost (cost)
  {
    assert(totalCost >= 0);
  }
  // getters
  const std::string&
  GetAirLineName () const
  {
    return AirLineName;
  }
  const std::string&
  GetDateTimeFrom () const
  {
    return DateTimeFrom;
  }
  const std::string&
  GetDateTimeTo () const
  {
    return DateTimeTo;
  }
  double
  GetTotalCost () const
  {
    return totalCost;
  }

  // setters
  Flight&
  SetAirLineName (const std::string &AirName)
  {
    this->AirLineName = AirName;
    return *this;
  }
  Flight&
  SetDateTimeFrom (const std::string &_DateTimeFrom)
  {
    DateTimeFrom = _DateTimeFrom;
    return *this;
  }
  Flight&
  SetDateTimeTo (const std::string &_DateTimeTo)
  {
    DateTimeTo = _DateTimeTo;
    return *this;
  }
  Flight&
  SetTotalCost (double cost)
  {
    assert(cost >= 0);
    totalCost = cost;
    return *this;
  }
  virtual std::string
  ToStr () const
  {
    ostringstream oss;
    oss << "Airline: " << GetAirLineName () << " Cost: " << GetTotalCost ()
	<< " Departure Date " << GetDateTimeFrom () << " Arrival date "
	<< GetDateTimeTo ();
    return oss.str ();
  }
};

class FlightRequst
{
private:
  std::string DateFrom;
  std::string DateTo;
  std::string TimeFrom;
  std::string TimeTo;
  std::string From;
  std::string To;
  int adults;
  int childern;
  int infants;
public:
  FlightRequst () = default;
//getters
  const std::string&
  GetFrom () const
  {
    return From;
  }
  const std::string&
  GetTo () const
  {
    return To;
  }
  const std::string&
  GetDateFrom () const
  {
    return DateFrom;
  }
  const std::string&
  GetDateTo () const
  {
    return DateTo;
  }
  const std::string&
  GetTimeFrom () const
  {
    return TimeFrom;
  }
  const std::string&
  GetTimeTo () const
  {
    return TimeTo;
  }
  int
  GetAdults () const
  {
    return adults;
  }
  int
  GetChildern () const
  {
    return childern;
  }
  int
  GetInfants () const
  {
    return infants;
  }

// setters
  FlightRequst&
  SetFrom (const std::string &from)
  {
    From = from;
    return *this;
  }
  FlightRequst&
  SetTo (const std::string &to)
  {
    To = to;
    return *this;
  }
  FlightRequst&
  SetDateFrom (const std::string &_DateFrom)
  {
    DateFrom = _DateFrom;
    return *this;
  }
  FlightRequst&
  SetDateTo (const std::string &_DateTo)
  {
    DateTo = _DateTo;
    return *this;
  }
  FlightRequst&
  SetTimeFrom (const std::string &_TimeFrom)
  {
    TimeFrom = _TimeFrom;
    return *this;
  }
  FlightRequst&
  SetTimeTo (const std::string &_TimeTo)
  {
    TimeFrom = _TimeTo;
    return *this;
  }

  FlightRequst&
  SetAdults (int _adults)
  {
    assert(_adults >= 0);
    adults = _adults;
    return *this;
  }
  FlightRequst&
  SetChildern (int _children)
  {
    assert(_children >= 0);
    childern = _children;
    return *this;
  }
  FlightRequst&
  SetInfants (int _infants)
  {
    assert(_infants >= 0);
    infants = _infants;
    return *this;
  }
};
class FlightReservation : public Reservation
{
private:
  Flight flight;
  FlightRequst req;

public:
  FlightReservation (const Flight &flight, const FlightRequst &req) :
      flight (flight), req (req)
  {
  }
  virtual double
  getTotalCost () const override
  {
    // cost of 1 chair * total number of required seats
    return (flight.GetTotalCost () * (req.GetAdults () + req.GetChildern ()));
  }
  virtual void
  print () const override
  {
    std::cout << "Airline reservation with reservation: "
	<< flight.GetAirLineName () << ": From " << req.GetFrom () << " on "
	<< req.GetDateFrom () << "\n";
    std::cout << "\tTo " << req.GetTo () << " on " << req.GetDateTo () << "\n";
    std::cout << "\t" << "Adults: " << req.GetAdults () << " children: "
	<< req.GetChildern () << " infants: " << req.GetInfants () << "\n";
    std::cout << "\tTotal Cost:" << getTotalCost () << "\n";
  }
  virtual std::string
  ToStr () const override
  {
    std::ostringstream oss;
    oss << "Airline reservation with reservation: " << flight.GetAirLineName ()
	<< ": From " << req.GetFrom () << " on " << req.GetDateFrom () << "\n";
    oss << "\tTo " << req.GetTo () << " on " << req.GetDateTo () << "\n";
    oss << "\t" << "Adults: " << req.GetAdults () << " children: "
	<< req.GetChildern () << " infants: " << req.GetInfants () << "\n";
    oss << "\tTotal Cost:" << getTotalCost () << "\n";

    return oss.str ();
  }
  virtual Reservation*
  clone () const override
  {
    return new FlightReservation (*this);
  }
  // getters
  const Flight&
  GetFlight () const
  {
    return flight;
  }
  const FlightRequst
  GetRequest () const
  {
    return req;
  }
};

#endif /* FLIGHTS_ */
