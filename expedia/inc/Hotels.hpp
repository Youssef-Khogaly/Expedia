/*
 * Hotels.hpp
 *
 *  Created on: Feb 23, 2025
 *      Author: khogaly
 */

#ifndef HOTELS_HPP_
#define HOTELS_HPP_
#include <Reservation_common.hpp>
#include <string>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>
namespace
{

  using std::string;
  using std::cout;
  class HotelRoom
  {
  private:
    string HotelName;
    string roomType;
    string dateFrom;
    string dateTo;
    double pricePerNight;
    int AvaliableRooms;
  public:
    /**
     * Date Fromat dd-mm-yyyy
     * */
    HotelRoom (const string &HotelName =
      { },
	       const string &roomType =
		 { },
	       const string &dateFrom =
		 { },
	       const string &dateTo =
		 { },
	       int AvaliableRooms =
		 { 0 },
	       double pricePerNight =
		 { 0.0 }) :
	HotelName (HotelName), roomType (roomType), dateFrom (dateFrom), dateTo (
	    dateTo), AvaliableRooms (AvaliableRooms), pricePerNight (
	    pricePerNight)
    {
    }

    // getters
    const string&
    GetHotelName () const
    {
      return HotelName;
    }
    const string&
    GetRoomType () const
    {
      return roomType;
    }
    const string&
    GetDateFrom () const
    {
      return dateFrom;
    }
    const string&
    GetDateTo () const
    {
      return dateTo;
    }
    double
    GetPricePerNight () const
    {
      return pricePerNight;
    }
    int
    GetAvaliableRooms () const
    {
      return AvaliableRooms;
    }
    // setters
    HotelRoom&
    SetHotelName (const string &name)
    {
      HotelName = name;
      return *this;
    }
    HotelRoom&
    SetRoomType (const string &type)
    {
      roomType = type;
      return *this;
    }
    /**
     * Date Fromat dd-mm-yyyy
     * */
    HotelRoom&
    SetDateFrom (const string &DateFrom)
    {
      dateFrom = DateFrom;
      return *this;
    }
    /**
     * Date Fromat dd-mm-yyyy
     * */
    HotelRoom&
    SetDateTo (const string &DateTo)
    {
      dateTo = DateTo;

      return *this;
    }
    HotelRoom&
    SetPricePerNight (double price)
    {
      pricePerNight = price;
      return *this;
    }
    HotelRoom&
    SetAvaliableRooms (int num_rooms)
    {
      AvaliableRooms = num_rooms;
      return *this;
    }

    string
    ToStr () const
    {
      ostringstream oss;
      oss << "Hotel: " << GetHotelName () << " Room Type: " << GetRoomType ()
	  << "(" << GetAvaliableRooms () << ") Price per night: "
	  << GetPricePerNight () << " From Date " << GetDateFrom () << " to "
	  << GetDateTo () << "";
      return oss.str ();
    }

  };

  class HotelRequest
  {
  private:
    string dateFrom;
    string dateTo;
    string country;
    string city;
    int adults;
    int children;
    int rooms;

    // day- (month-1) - (year-1900)
    std::tuple<int, int, int>
    ParseDate (const string &Date, const string &Format = "%d-%m-%y") const
    {

      std::stringstream ss (Date);
      std::tm tm =
	{ };
      ss >> get_time (&tm, Format.c_str ());
      if (!ss.eof ())
	throw std::invalid_argument ("invalid Date Fromat");
      return
	{ tm.tm_mday , tm.tm_mon ,tm.tm_year};
    }
  public:
    HotelRequest () :
	dateFrom (string
	  { }), dateTo (string
	  { }), country (string
	  { }), city (string
	  { }), adults (0), children (0), rooms (0)
    {
    }

    //getters
    int
    GetTotalNights () const
    {
      auto [d1, m1, y1] = ParseDate (GetDateFrom ());
      auto [d2, m2, y2] = ParseDate (GetDateTo ());
      std::tm from =
	{ 0, 0, 0, d1, m1, y1 };
      std::tm to =
	{ 0, 0, 0, d2, m2, y2 };
      std::time_t tFrom = std::mktime (&from);
      std::time_t tTo = std::mktime (&to);

      double diff = std::difftime (tTo, tFrom);
      diff = (diff) / (60 * 60 * 24);

      return (ceil (diff));
    }
    const string&
    GetDateFrom () const
    {
      return dateFrom;
    }
    const string&
    GetDateTo () const
    {
      return dateTo;
    }
    const string&
    GetCountry () const
    {
      return country;
    }
    const string&
    GetCity () const
    {
      return city;
    }

    int
    GetAdults () const
    {
      return adults;
    }
    int
    GetChildren () const
    {
      return children;
    }
    int
    GetRooms () const
    {
      return rooms;
    }

// setters
    HotelRequest&
    SetDateFrom (const string &_dateFrom)
    {
      dateFrom = _dateFrom;
      return *this;
    }
    HotelRequest&
    SetDateTo (const string &_dateTo)
    {
      dateTo = _dateTo;
      return *this;
    }
    HotelRequest&
    SetCountry (const string &_country)
    {

      country = _country;
      return *this;
    }
    HotelRequest&
    SetCity (const string &_city)
    {

      city = _city;
      return *this;
    }

    HotelRequest&
    SetAdults (int adult)
    {
      assert (adult >= 0);
      adults = adult;
      return *this;
    }
    HotelRequest&
    SetChildren (int child)
    {
      assert (child >= 0);
      children = child;
      return *this;
    }
    HotelRequest&
    SetRooms (int _rooms)
    {
      assert (_rooms >= 0);
      rooms = _rooms;
      return *this;
    }
  };

  class HotelReservation : public Reservation
  {
  private:
    HotelRoom room;
    HotelRequest req;
  public:
    HotelReservation (HotelRoom room, HotelRequest req) :
	room (room), req (req)
    {

    }
    const HotelRoom&
    GetRoom () const
    {
      return room;
    }
    const HotelRequest&
    GetRequest () const
    {
      return req;
    }
    virtual Reservation*
    clone () const override
    {
      return new HotelReservation (*this);
    }
    virtual void
    print () const
    {
      cout << "Hotel reservation: " << room.GetHotelName () << ": "
	  << req.GetCity () << " @ " << req.GetCountry () << "\n";
      cout << "\t" << req.GetDateFrom () << " to " << req.GetDateTo () << " : "
	  << req.GetTotalNights () << "\n";
      cout << "\t" << "Adults: " << req.GetAdults () << " children "
	  << req.GetChildren () << "\n";
      cout << "\tTotal Cost:" << getTotalCost () << "\n";

    }
    virtual std::string
    ToStr () const override
    {
      ostringstream oss;
      oss << "Hotel reservation: " << room.GetHotelName () << ": "
	  << req.GetCity () << " @ " << req.GetCountry () << "\n";
      oss << "\t" << req.GetDateFrom () << " to " << req.GetDateTo () << " : "
	  "total Nights:" << req.GetTotalNights () << "\n";
      oss << "number of rooms: " << req.GetRooms () << "\n";
      oss << "\t" << "Adults: " << req.GetAdults () << " children "
	  << req.GetChildren () << "\n";
      oss << "\tTotal Cost:" << getTotalCost () << "\n";

      return oss.str ();
    }
    virtual double
    getTotalCost () const override
    {
      return (req.GetRooms () * room.GetPricePerNight () * req.GetTotalNights ());
    }
  };
}

#endif /* HOTELS_HPP_ */
