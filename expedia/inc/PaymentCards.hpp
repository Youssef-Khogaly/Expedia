/*
 * PaymentCards.hpp
 *
 *  Created on: Feb 26, 2025
 *      Author: khogaly
 */

#ifndef PAYMENTCARDS_HPP_
#define PAYMENTCARDS_HPP_

#include <string>
#include <sstream>

namespace
{
  using std::string;
  using std::ostringstream;

  class PaymentCard
  {
  protected:
    string name;
    string cardNum;
    string exp_date;
    int ccv;
  public:
    //getters
    const string&
    GetName () const
    {
      return name;
    }
    const string&
    GetCardNumber () const
    {
      return cardNum;
    }
    const string&
    GetExp_date () const
    {
      return exp_date;
    }
    int
    GetCCV () const
    {
      return ccv;
    }
    // setters
    void
    SetName (const string &_name)
    {
      name = _name;
    }
    void
    SetCardNumber (const string &num)
    {
      cardNum = num;
    }
    void
    Set_ExpDate (const string &date)
    {
      exp_date = date;
    }
    void
    Set_CCV (int CCV)
    {
      ccv = CCV;
    }

    virtual string
    ToStr () const
    {
      ostringstream oss;
      oss << "Owner: " << name << " Card number: " << cardNum
	  << " Expiry date: " << exp_date;
      return oss.str ();
    }
    virtual PaymentCard*
    clone () const
    {
      return new PaymentCard (*this);
    }

    virtual
    ~PaymentCard () = default;
  };

  class DebitCard : public PaymentCard
  {
  private:
    string BillingAddress;
  public:
    const string&
    GetBillingAddress () const
    {
      return BillingAddress;
    }
    DebitCard&
    SetBillingAddress (const string &address)
    {
      BillingAddress = address;
      return *this;
    }
    virtual string
    ToStr () const override
    {
      ostringstream oss;
      oss << "[Debit Card]:" << PaymentCard::ToStr () << " Billing Address"
	  << BillingAddress;
      return oss.str ();
    }
    virtual PaymentCard*
    clone () const override
    {
      return new DebitCard (*this);
    }
  };

  class CreditCard : public PaymentCard
  {
  public:
    virtual string
    ToStr () const override
    {
      ostringstream oss;
      oss << "[Credit Card]:" << PaymentCard::ToStr ();
      return oss.str ();
    }
    virtual PaymentCard*
    clone () const override
    {
      return new CreditCard (*this);
    }
  };
}

#endif /* PAYMENTCARDS_HPP_ */
