/*
 * HotelManager.hpp
 *
 *  Created on: Feb 24, 2025
 *      Author: khogaly
 */

#ifndef HOTELMANAGER_HPP_
#define HOTELMANAGER_HPP_
#include <Hotels.hpp>
#include <expedia_hotels_api.hpp>
#include <vector>
#include <map>
namespace
{
  using std::vector;
  using std::string;
  using std::map;
  class IHotelManager
  {

  protected:
    HotelRequest hotelReq;
  public:

    IHotelManager () :
	hotelReq ()
    {

    }
    explicit
    IHotelManager (const HotelRequest &hotelReq) :
	hotelReq (hotelReq)
    {

    }
    virtual void
    SetHotelRequest (const HotelRequest &Req) = 0;
    virtual vector<HotelRoom>
    SearchHotels () const = 0;
    virtual bool
    ReserveHotel (const HotelRoom&) = 0;
    virtual bool
    CancelReservation (const HotelReservation&) = 0;
    virtual string
    GetHotelName () const = 0;

    virtual
    ~IHotelManager () = default;
  };

  class MarriottHotel_Manager : public IHotelManager
  {

  public:
    MarriottHotel_Manager () :
	IHotelManager ()
    {

    }
    virtual void
    SetHotelRequest (const HotelRequest &Req) override
    {
      hotelReq = Req;
    }
    virtual vector<HotelRoom>
    SearchHotels () const override
    {
      vector<HotelRoom> FoundedRooms
	{ };
      auto Mariottrooms = MarriottHotelAPI::FindRooms (hotelReq.GetDateFrom (),
						       hotelReq.GetDateTo (),
						       hotelReq.GetCountry (),
						       hotelReq.GetCity (),
						       hotelReq.GetRooms (),
						       hotelReq.GetAdults (),
						       hotelReq.GetChildren ());
      FoundedRooms.reserve (Mariottrooms.size ());
      for (const auto &room : Mariottrooms)
	{
	  FoundedRooms.push_back (HotelRoom
	    { "Marriott Hotel", room.room_type, room.date_from, room.date_to,
		room.available, room.price_per_night });
	}

      return FoundedRooms;
    }
    virtual bool
    ReserveHotel (const HotelRoom &Room) override
    {
      MarriottFoundRoom MarriottRoom;
      MarriottRoom.date_from = Room.GetDateFrom ();
      MarriottRoom.date_to = Room.GetDateTo ();
      MarriottRoom.available = Room.GetAvaliableRooms ();
      MarriottRoom.price_per_night = Room.GetPricePerNight ();
      return MarriottHotelAPI::reserveRoom (MarriottRoom);
    }
    virtual bool
    CancelReservation (const HotelReservation &reserve) override
    {
      const auto &Room = reserve.GetRoom ();
      MarriottFoundRoom MarriottRoom;
      MarriottRoom.date_from = Room.GetDateFrom ();
      MarriottRoom.date_to = Room.GetDateTo ();
      MarriottRoom.available = Room.GetAvaliableRooms ();
      MarriottRoom.price_per_night = Room.GetPricePerNight ();
      return MarriottHotelAPI::CancelRoom (MarriottRoom);
    }
    virtual string
    GetHotelName () const override
    {
      return "Marriott Hotel";
    }
  };

  class HiltonHotel_Manager : public IHotelManager
  {

  public:
    HiltonHotel_Manager () :
	IHotelManager ()
    {

    }
    virtual void
    SetHotelRequest (const HotelRequest &Req) override
    {
      hotelReq = Req;
    }
    virtual vector<HotelRoom>
    SearchHotels () const override
    {
      auto HiltonRooms = HiltonHotelAPI::SearchRooms (hotelReq.GetCountry (),
						      hotelReq.GetCity (),
						      hotelReq.GetDateFrom (),
						      hotelReq.GetDateTo (),
						      hotelReq.GetAdults (),
						      hotelReq.GetChildren (),
						      hotelReq.GetRooms ());
      vector<HotelRoom> rooms;
      rooms.reserve (HiltonRooms.size ());

      for (const auto &room : HiltonRooms)
	{
	  rooms.push_back (
	      HotelRoom
		{ "Hilton Hotel", room.room_type, room.date_from, room.date_to,
		    room.available, room.price_per_night });
	}

      return rooms;

    }
    virtual bool
    ReserveHotel (const HotelRoom &room) override
    {
      HiltonRoom hiltonRoom;
      hiltonRoom.date_from = room.GetDateFrom ();
      hiltonRoom.date_to = room.GetDateTo ();
      hiltonRoom.room_type = room.GetRoomType ();
      hiltonRoom.available = room.GetAvaliableRooms ();
      hiltonRoom.price_per_night = room.GetPricePerNight ();
      return HiltonHotelAPI::reserveRoom (hiltonRoom);
    }
    virtual bool
    CancelReservation (const HotelReservation &reserve) override
    {
      const auto &room = reserve.GetRoom ();
      HiltonRoom hiltonRoom;
      hiltonRoom.date_from = room.GetDateFrom ();
      hiltonRoom.date_to = room.GetDateTo ();
      hiltonRoom.room_type = room.GetRoomType ();
      hiltonRoom.available = room.GetAvaliableRooms ();
      hiltonRoom.price_per_night = room.GetPricePerNight ();
      return HiltonHotelAPI::CancelRoom (hiltonRoom);
    }
    virtual string
    GetHotelName () const override
    {
      return "Hilton Hotel";
    }
  };

  /*
   *
   * This class reduce the dependency. This is the only place that is coupled with the different types  and it create single object of each hotel manager only
   * */
  class HotelFactory
  {
  private:
    inline static map<string, IHotelManager*> HotelName_Manager_Map
      { };

    static void
    inisalization ()
    {

      if (!HotelName_Manager_Map.empty ())
	{
	  clear ();
	  HotelName_Manager_Map.clear ();
	}
      MarriottHotel_Manager *MarriotPtr = new MarriottHotel_Manager;
      HiltonHotel_Manager *HiltonPtr = new HiltonHotel_Manager;
      HotelName_Manager_Map.insert (
	{ MarriotPtr->GetHotelName (), MarriotPtr });
      HotelName_Manager_Map.insert (
	{ HiltonPtr->GetHotelName (), HiltonPtr });
    }
  public:
    HotelFactory () = delete;
    HotelFactory (const HotelFactory&) = delete;
    HotelFactory&
    operator= (const HotelFactory&) = delete;

    static std::map<string, IHotelManager*>&
    GetManagersMap ()
    {
      if (HotelName_Manager_Map.empty ())
	inisalization ();

      return HotelName_Manager_Map;
    }
    static std::vector<IHotelManager*>
    GetManagersVector ()
    {
      vector<IHotelManager*> ret
	{ };
      ret.reserve (HotelName_Manager_Map.size ());

      if (HotelName_Manager_Map.empty ())
	inisalization ();
      for (auto& [name, ptr] : HotelName_Manager_Map)
	ret.push_back (ptr);

      return ret;
    }
    static IHotelManager*
    GetManager (const string &name)
    {
      if (HotelName_Manager_Map.empty ())
	inisalization ();

      return HotelName_Manager_Map.at (name);
    }
    static void
    clear ()
    {
      for (auto& [name, mgr] : HotelName_Manager_Map)
	{
	  if (mgr != nullptr)
	    {
	      delete mgr;
	      mgr = nullptr;
	    }
	}
      HotelName_Manager_Map.clear ();
    }
  };

}

#endif /* HOTELMANAGER_HPP_ */
