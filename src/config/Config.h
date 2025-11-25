#ifndef CONFIG_H
#define CONFIG_H

class Config
{
private:
  double expressSurchargeRate;
  double photoPrintingBasePrice;
  double filmDevelopingBasePrice;

public:
  Config();

  // Getters for business rules
  double getExpressSurchargeRate() const;
  double getPhotoPrintingBasePrice() const;
  double getFilmDevelopingBasePrice() const;

  // Setters for runtime configuration (if needed)
  void setExpressSurchargeRate(double rate);
  void setPhotoPrintingBasePrice(double price);
  void setFilmDevelopingBasePrice(double price);
};

#endif // CONFIG_H
