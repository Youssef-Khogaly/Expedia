/*
 * Interface_FlightsManager.hpp
 *
 *  Created on: Feb 23, 2025
 *      Author: khogaly
 */

#ifndef FLIGHTSMANAGER_HPP_
#define FLIGHTSMANAGER_HPP_
#include <vector>
#include <string>
#include <Flights.hpp>
#include <expedia_flights_api.hpp>
#include <map>
class IFlightsManager
{
protected:
  FlightRequst FlightReq;

public:
  IFlightsManager () :
      FlightReq ()
  {

  }
  explicit
  IFlightsManager (const FlightRequst &FlgihtReq) :
      FlightReq (FlightReq)
  {

  }
  virtual void
  SetFlightRequst (const FlightRequst &Requst)
  {
    FlightReq = Requst;
  }
  virtual std::vector<Flight>
  GetFlights () const = 0;
  virtual bool
  ReserveFlight (const Flight &_Flight) const = 0;
  virtual bool
  CancelFlight (const FlightReservation &FlightReserve) const = 0;
  virtual std::string
  GetAirLineName () const = 0;

  virtual
  ~IFlightsManager () = default;

};

class AirCanadaOnlineAPI_Manager : public IFlightsManager
{
public:
  // user IFlightConstructor
  using IFlightsManager::IFlightsManager;
  virtual std::vector<Flight>
  GetFlights () const override
  {
    std::vector<Flight> ret
      { };
    auto &req = FlightReq;
    auto AirCanadaFlights = AirCanadaOnlineAPI::GetFlights (req.GetDateFrom (),
							    req.GetDateTo (),
							    req.GetTimeFrom (),
							    req.GetTimeTo (),
							    req.GetAdults (),
							    req.GetChildern ());
    ret.reserve (AirCanadaFlights.size ());
    for (auto &flight : AirCanadaFlights)
      {
	ret.push_back (Flight
	  { GetAirLineName (), flight.date_time_from, flight.date_time_to,
	      flight.price });
      }

    return ret;
  }
  virtual std::string
  GetAirLineName () const override
  {
    return
      { "Air Canada"};
  }
  virtual bool
  ReserveFlight (const Flight &_Flight) const override
  {
    AirCanadaCustomerInfo customer
      { }; // just dummy
    AirCanadaFlight CanadaFlight
      { };
    CanadaFlight.date_time_from = _Flight.GetDateTimeFrom ();
    CanadaFlight.date_time_to = _Flight.GetDateTimeTo ();
    CanadaFlight.price = _Flight.GetTotalCost ();

    return AirCanadaOnlineAPI::ReserveFlight (CanadaFlight, customer);
  }
  virtual bool
  CancelFlight (const FlightReservation &FlightReserve) const override
  {
    const auto &_Flight = FlightReserve.GetFlight ();
    AirCanadaCustomerInfo customer
      { };
    AirCanadaFlight CanadaFlight
      { };
    CanadaFlight.date_time_from = _Flight.GetDateTimeFrom ();
    CanadaFlight.date_time_to = _Flight.GetDateTimeTo ();
    CanadaFlight.price = _Flight.GetTotalCost ();
    return AirCanadaOnlineAPI::CancelFlight (CanadaFlight, customer);
  }
  ~AirCanadaOnlineAPI_Manager () = default;
};

class TurkishAirlinesOnlineAPI_Manager : public IFlightsManager
{
private:
public:
  using IFlightsManager::IFlightsManager;
  virtual std::vector<Flight>
  GetFlights () const
  {
    TurkishAirlinesOnlineAPI TurkishApi
      { };
    std::vector<Flight> ret
      { };
    auto AvaliableFlights = TurkishApi.GetAvailableFlights ();
    ret.reserve (AvaliableFlights.size ());

    for (const auto &AvFlight : AvaliableFlights)
      {
	ret.push_back (Flight
	  { "Turkish Airline", AvFlight.datetime_from, AvFlight.datetime_to,
	      AvFlight.cost });
      }
    return ret;
  }
  virtual bool
  ReserveFlight (const Flight &_Flight) const
  {
    TurkishFlight TurkFlight
      { };
    TurkishCustomerInfo customer; // dummpy we should add customer info
    TurkFlight.datetime_from = _Flight.GetDateTimeFrom ();
    TurkFlight.datetime_to = _Flight.GetDateTimeTo ();
    TurkFlight.cost = _Flight.GetTotalCost ();
    return TurkishAirlinesOnlineAPI::ReserveFlight (customer, TurkFlight);

  }
  virtual bool
  CancelFlight (const FlightReservation &FlightReserve) const
  {
    const auto &_Flight = FlightReserve.GetFlight ();
    TurkishFlight TurkFlight
      { };
    TurkishCustomerInfo customer; // dummpy we should add customer info
    TurkFlight.datetime_from = _Flight.GetDateTimeFrom ();
    TurkFlight.datetime_to = _Flight.GetDateTimeTo ();
    TurkFlight.cost = _Flight.GetTotalCost ();
    return TurkishAirlinesOnlineAPI::CancelFlight (customer, TurkFlight);
  }
  virtual std::string
  GetAirLineName () const
  {
    return "Turkish Airline";
  }
};

/*
 * this class reduce dependency of clients on specific airline manager
 *
 * this is the only place coupled with diffrent types
 * */
class FlightsFactory
{
private:
  inline static std::map<string, IFlightsManager*> AirLineName_Manager_map
    { };
  void
  static
  inisalization ()
  {
    if (!AirLineName_Manager_map.empty ())
      {
	clear ();
	AirLineName_Manager_map.clear ();
      }
    AirCanadaOnlineAPI_Manager *CanadanManager = new AirCanadaOnlineAPI_Manager;
    TurkishAirlinesOnlineAPI_Manager *TurkManager =
	new TurkishAirlinesOnlineAPI_Manager;

    AirLineName_Manager_map.insert (
      { CanadanManager->GetAirLineName (), CanadanManager });
    AirLineName_Manager_map.insert (
      { TurkManager->GetAirLineName (), TurkManager });
  }
public:
  FlightsFactory () = delete;
  FlightsFactory (const FlightsFactory&) = delete; // copy constructor
  FlightsFactory&
  operator= (const FlightsFactory&) = delete;  // copy assign
  static std::map<string, IFlightsManager*>&
  GetManagersMap ()
  {
    if (AirLineName_Manager_map.empty ())
      {
	FlightsFactory::inisalization ();
      }
    return FlightsFactory::AirLineName_Manager_map;
  }
  static std::vector<IFlightsManager*>
  GetManagersVector ()
  {
    std::vector<IFlightsManager*> ret;
    ret.reserve (AirLineName_Manager_map.size ());
    if (AirLineName_Manager_map.empty ())
      {
	FlightsFactory::inisalization ();
      }
    for (auto& [name, ptr] : AirLineName_Manager_map)
      {
	ret.push_back (ptr);
      }
    return ret;
  }
  /*
   * thorw std::out_of_range if there is not manager for that airline
   * */
  static IFlightsManager*
  GetManager (const std::string &AirLineName)
  {
    auto &map = FlightsFactory::GetManagersMap ();
    return map.at (AirLineName);
  }
  static void
  clear ()
  {
    for (auto& [name, mgr] : AirLineName_Manager_map)
      {
	if (mgr != nullptr)
	  {
	    delete mgr;
	    mgr = nullptr;
	  }
      }
    AirLineName_Manager_map.clear ();
  }
};

#endif /* INTERFACE_FLIGHTSMANAGER_HPP_ */
