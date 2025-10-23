#include "Config.h"

Config::Config()
    : expressSurchargeRate(0.25), // 25% surcharge for express orders
      photoPrintingBasePrice(15.99),
      filmDevelopingBasePrice(25.50)
{
}

double Config::getExpressSurchargeRate() const
{
  return expressSurchargeRate;
}

double Config::getPhotoPrintingBasePrice() const
{
  return photoPrintingBasePrice;
}

double Config::getFilmDevelopingBasePrice() const
{
  return filmDevelopingBasePrice;
}

void Config::setExpressSurchargeRate(double rate)
{
  expressSurchargeRate = rate;
}

void Config::setPhotoPrintingBasePrice(double price)
{
  photoPrintingBasePrice = price;
}

void Config::setFilmDevelopingBasePrice(double price)
{
  filmDevelopingBasePrice = price;
}
