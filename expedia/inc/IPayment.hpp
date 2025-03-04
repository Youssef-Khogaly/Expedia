/*
 * IPayment.hpp
 *
 *  Created on: Feb 26, 2025
 *      Author: khogaly
 */

#ifndef IPAYMENT_HPP_
#define IPAYMENT_HPP_
#include <string>
#include <json.hpp>
#include <expedia_payments_api.hpp>
#include <cassert>
#include <array>
namespace
{
  using std::array;
  using std::vector;
  using std::string;
  enum class PaymentMethod
  {
    Paypal = 0, Stripe = 1, Square = 2
  };

  class IPayment
  {

  private:

  public:
    virtual void
    SetUserInfo (const string &name, const string &address) = 0;
    virtual void
    SetCardInfo (const string &id, const string &expDate, int ccv) = 0;
    virtual bool
    MakePayment (double money) = 0;
    virtual
    ~IPayment () = default;
  };

  class PayPal_Payment : public IPayment
  {
  private:
    PayPalCreditCard card;
  public:
    virtual void
    SetUserInfo (const string &name, const string &address) override
    {
      card.name = name;
      card.address = address;
    }
    virtual void
    SetCardInfo (const string &id, const string &expDate, int ccv) override
    {
      card.id = id;
      card.expiry_date = expDate;
      card.ccv = ccv;
    }
    virtual bool
    MakePayment (double money) override
    {
      assert(money >= 0);
      PayPalOnlinePaymentAPI api;
      api.SetCardInfo (&card);

      return api.MakePayment (money);
    }
  };
  class Stripe_Payment : public IPayment
  {
  private:
    StripeUserInfo usr;
    StripeCardInfo card;
  public:
    virtual void
    SetUserInfo (const string &name, const string &address) override
    {
      usr.name = name;
      usr.address = address;
    }
    virtual void
    SetCardInfo (const string &id, const string &expDate, int ccv) override
    {
      card.id = id;
      card.expiry_date = expDate;
    }
    virtual bool
    MakePayment (double money) override
    {
      return StripePaymentAPI::WithDrawMoney (usr, card, money);
    }
  };
  class Square_Payment : public IPayment
  {
  private:
    json::JSON js;
  public:
    virtual void
    SetUserInfo (const string &name, const string &address) override
    {
      js["user_info"] = json::Array (name, address);
    }
    virtual void
    SetCardInfo (const string &id, const string &expDate, int ccv) override
    {
      js["card_info"]["ID"] = id;
      js["card_info"]["DATE"] = expDate;
      js["card_info"]["CCV"] = ccv;

    }
    virtual bool
    MakePayment (double money) override
    {
      js["money"] = money;
      return SquarePaymentAPI::WithDrawMoney (js.dump ());
    }
  };

  class PaymentSingletonsFactory
  {
  private:
    inline static array<IPayment*, 3> Methods
      {
	{ nullptr, nullptr, nullptr } };
  public:
    PaymentSingletonsFactory () = delete;
    PaymentSingletonsFactory (const PaymentSingletonsFactory&) = delete;
    PaymentSingletonsFactory&
    operator= (PaymentSingletonsFactory&) = delete;

    static IPayment*
    GetPaymentHelper (PaymentMethod method)
    {
      int idx = static_cast<int> (method);
      if (nullptr == Methods.at (idx))
	{
	  switch (method)
	    {
	    case PaymentMethod::Paypal:

	      Methods.at (idx) = new PayPal_Payment;
	      break;
	    case PaymentMethod::Square:

	      Methods.at (idx) = new Square_Payment;
	      break;
	    case PaymentMethod::Stripe:
	      Methods.at (idx) = new Stripe_Payment;
	      break;
	    default:
	      throw std::invalid_argument ("invalid payment method");
	      break;
	    }
	}

      return Methods.at (idx);
    }

    static void
    clear () noexcept
    {
      for (auto &method : Methods)
	{
	  if (method)
	    {
	      delete method;
	      method = nullptr;
	    }
	}
    }
  };
}
;

#endif /* IPAYMENT_HPP_ */
