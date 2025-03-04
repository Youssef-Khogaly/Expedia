/*
 * ExpediaBackEnd.hpp
 *
 *  Created on: Feb 27, 2025
 *      Author: khogaly
 */

#ifndef EXPEDIABACKEND_HPP_
#define EXPEDIABACKEND_HPP_
#include <vector>
#include <FlightsManager.hpp>
#include <HotelManager.hpp>
#include <IPayment.hpp>
#include <Flights.hpp>
#include<Hotels.hpp>
#include <PaymentCards.hpp>
namespace
{
  using std::vector;
  class BackEnd
  {
  private:
    vector<IFlightsManager*> FlightMgrs;
    vector<IHotelManager*> HotelMgrs;
    IPayment *PaymentHelper;

    void
    ClearMgrs ()
    {
      FlightsFactory::clear ();
      HotelFactory::clear ();
      PaymentSingletonsFactory::clear ();
      FlightMgrs.clear ();
      HotelMgrs.clear ();
      PaymentHelper = nullptr;
    }

  public:
    BackEnd () :
	FlightMgrs (FlightsFactory::GetManagersVector ()), HotelMgrs (
	    HotelFactory::GetManagersVector ()), PaymentHelper (nullptr)
    {
    }
    BackEnd (const BackEnd&) = delete;
    BackEnd (BackEnd&&) = delete;
    BackEnd&
    operator = (const BackEnd&) = delete;
    BackEnd&
    operator = (BackEnd&&) = delete;
    ~BackEnd ()
    {
      ClearMgrs ();
    }

    vector<Flight>
    FindFlight (const FlightRequst &Req) const
    {
      vector<Flight> ret
	{ };

      ret.reserve (16);
      for (auto &pMgr : FlightMgrs)
	{
	  pMgr->SetFlightRequst (Req);
	  auto AvaliableFlights = pMgr->GetFlights ();
	  ret.insert (ret.end (),
		      std::make_move_iterator (AvaliableFlights.begin ()),
		      std::make_move_iterator (AvaliableFlights.end ()));
	}
      ret.shrink_to_fit ();
      return ret;
    }
    vector<HotelRoom>
    FindHotel (const HotelRequest &Req) const
    {
      vector<HotelRoom> ret
	{ };

      ret.reserve (16);
      for (auto &pMgr : HotelMgrs)
	{
	  pMgr->SetHotelRequest (Req);
	  auto AvaliableHotels = pMgr->SearchHotels ();
	  ret.insert (ret.end (),
		      std::make_move_iterator (AvaliableHotels.begin ()),
		      std::make_move_iterator (AvaliableHotels.end ()));
	}
      ret.shrink_to_fit ();
      return ret;
    }
    bool
    ConfirmReservation (const Reservation &reserveation)
    {
      Reservation *res_cpy = reserveation.clone ();
      HotelReservation *HotelRes = dynamic_cast<HotelReservation*> (res_cpy);
      FlightReservation *FlightRes = dynamic_cast<FlightReservation*> (res_cpy);
      ItenaryReservation *ItenaryRes =
	  dynamic_cast<ItenaryReservation*> (res_cpy);
      string MgrName
	{ };
      bool ret = false;
      MgrName.reserve (32);
      if (FlightRes)
	{
	  MgrName = FlightRes->GetFlight ().GetAirLineName ();
	  auto Mgr = FlightsFactory::GetManager (MgrName);
	  ret = Mgr->ReserveFlight (FlightRes->GetFlight ());
	  ret = true;
	}
      else if (HotelRes)
	{
	  MgrName = HotelRes->GetRoom ().GetHotelName ();
	  auto Mgr = HotelFactory::GetManager (MgrName);
	  ret = Mgr->ReserveHotel (HotelRes->GetRoom ());
	  ret = true;
	}
      else if (ItenaryRes)
	{
	  const auto &reservations_vec = ItenaryRes->GetReservations ();
	  vector<Reservation*> AcceptedReservations
	    { };
	  bool allAccepted = true;
	  AcceptedReservations.reserve (reservations_vec.size ());
	  for (const auto &res : reservations_vec)
	    {
	      allAccepted &= ConfirmReservation (*res);
	      if (!allAccepted)
		break;
	      AcceptedReservations.push_back (res);
	    }
	  if (!allAccepted)
	    {

	      for (const auto &res : AcceptedReservations)
		{
		  CancelReservation (*res);

		}
	      ret = false;
	    }
	  else
	    ret = true;
	}
      else
	{
	  assert(0);
	}
      delete res_cpy;
      res_cpy = nullptr;
      return ret;
    }
    bool
    CancelReservation (const Reservation &res) const
    {
      Reservation *res_cpy = res.clone ();
      HotelReservation *HotelRes = dynamic_cast<HotelReservation*> (res_cpy);
      FlightReservation *FlightRes = dynamic_cast<FlightReservation*> (res_cpy);
      ItenaryReservation *ItenaryRes =
	  dynamic_cast<ItenaryReservation*> (res_cpy);
      string MgrName
	{ };
      bool ret = false;
      MgrName.reserve (32);
      if (FlightRes)
	{
	  MgrName = FlightRes->GetFlight ().GetAirLineName ();
	  auto Mgr = FlightsFactory::GetManager (MgrName);
	  ret = Mgr->CancelFlight (*FlightRes);
	}
      else if (HotelRes)
	{
	  MgrName = HotelRes->GetRoom ().GetHotelName ();
	  auto Mgr = HotelFactory::GetManager (MgrName);
	  ret = Mgr->CancelReservation (*HotelRes);
	}
      else if (ItenaryRes)
	{
	  const auto &reservations_vec = ItenaryRes->GetReservations ();

	  bool allCanceled = true;
	  for (const auto &res : reservations_vec)
	    {
	      allCanceled &= CancelReservation (*res);
	    }
	  ret = allCanceled;
	}
      else
	{
	  assert(0);
	}
      delete res_cpy;
      res_cpy = nullptr;
      return ret;
    }
    bool
    UnChargeCost (double price, PaymentCard &card, PaymentMethod method)
    {
      // ToDo
      return true;
    }
    bool
    ChargeCost (double price, PaymentCard &card, PaymentMethod method)
    {
      CreditCard *creditCard = dynamic_cast<CreditCard*> (&card);
      DebitCard *debitCard = dynamic_cast<DebitCard*> (&card);
      PaymentHelper = PaymentSingletonsFactory::GetPaymentHelper (method);
      bool ret = false;
      if (creditCard)
	{
	  PaymentHelper->SetUserInfo (creditCard->GetName (), "");

	  PaymentHelper->SetCardInfo (creditCard->GetCardNumber (),
				      creditCard->GetExp_date (),
				      creditCard->GetCCV ());
	  ret = PaymentHelper->MakePayment (price);
	}
      else if (debitCard)
	{
	  PaymentHelper->SetUserInfo (debitCard->GetName (),
				      debitCard->GetBillingAddress ());

	  PaymentHelper->SetCardInfo (debitCard->GetCardNumber (),
				      debitCard->GetExp_date (),
				      debitCard->GetCCV ());
	  ret = PaymentHelper->MakePayment (price);
	}
      else
	assert(0);

      return ret;

    }

  }
  ;
}

#endif /* EXPEDIABACKEND_HPP_ */
