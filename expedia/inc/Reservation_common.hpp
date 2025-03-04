/*
 * Reservation_common.hpp
 *
 *  Created on: Feb 23, 2025
 *      Author: khogaly
 */

#ifndef RESERVATION_COMMON_HPP_
#define RESERVATION_COMMON_HPP_
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
class Reservation
{
public:
  virtual double
  getTotalCost () const = 0;
  virtual void
  print () const = 0;
  virtual std::string
  ToStr () const = 0;
  virtual Reservation*
  clone () const = 0;
  virtual
  ~Reservation ()
  {

  }
};

class ItenaryReservation : public Reservation
{
private:
  std::vector<Reservation*> reservations; // * to allow Polymorphism

public:
  ItenaryReservation () :
      reservations (std::vector<Reservation*>
	{ })
  {

  }
  // copy constructor
  ItenaryReservation (const ItenaryReservation &_other)
  {
    reservations.reserve (_other.reservations.size ());
    for (const Reservation *pReservation : _other.reservations)
      {
	bool tmp = this->AddReservation (*pReservation);
      }
  }
  ItenaryReservation
  operator= (const ItenaryReservation&) = delete; // copy assi
  // move constructor
  ItenaryReservation (const ItenaryReservation &&_other)
  {
    cout << "Itneary reservation move constructor\n\n";
    reservations = _other.reservations;

    for (auto pReservation : _other.reservations)
      pReservation = nullptr;
  }
  // move assign
  ItenaryReservation&
  operator = (ItenaryReservation &&_other)
  {
    cout << "Itneary reservation move assign\n\n";
    auto &me = *this;
    // handle self assignment
    if (this == &_other)
      return *this;

    // freeing our resources
    me.~ItenaryReservation ();
    // copy pointers
    me.reservations = _other.reservations;

    // nulling other pointers
    for (auto pReservation : _other.reservations)
      pReservation = nullptr;

    return me;
  }
  bool
  AddReservation (const Reservation &reservation)
  {
    reservations.push_back (reservation.clone ());
    return true;
  }
  bool
  CancelReservation (const size_t idx)
  {
    if (idx >= reservations.size ())
      {
	throw std::out_of_range ("Invalid Reservation Number\n");
	return false;
      }

    std::vector<Reservation*>::iterator it = reservations.begin () + idx;
    // delete pointed to memory
    delete (*it);
    (*it) = nullptr;

    reservations.erase (it);

    return true;
  }
  const std::vector<Reservation*>&
  GetReservations () const
  {
    return reservations;
  }
  virtual double
  getTotalCost () const override
  {
    double ret
      { 0.0 };
    for (const auto &res : reservations)
      ret += res->getTotalCost ();

    return ret;
  }
  virtual void
  print () const override
  {
    int i
      { 1 };
    for (const auto &res : reservations)
      {
	std::cout << i++ << "-";
	res->print ();
	std::cout << "\n";
      }
  }

  virtual std::string
  ToStr () const override
  {
    std::ostringstream oss;
    for (const auto &res : reservations)
      {
	oss << res->ToStr () << "\n";
      }

    return oss.str ();
  }
  virtual Reservation*
  clone () const override
  {
    return new ItenaryReservation (*this);
  }
  void
  ClearList ()
  {
    for (auto &res : reservations)
      {
	if (res != nullptr)
	  {
	    delete res;
	    res = nullptr;
	  }
      }
    reservations.clear ();
  }
  virtual
  ~ItenaryReservation ()
  {
    for (auto &res : reservations)
      {
	if (res != nullptr)
	  {
	    delete res;
	    res = nullptr;
	  }
      }
    reservations.clear ();
  }
};

class ReservationSet : public ItenaryReservation
{
public:
  virtual Reservation*
  clone () const override
  {
    return new ReservationSet (*this);
  }
};
#endif /* RESERVATION_COMMON_HPP_ */
