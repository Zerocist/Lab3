#include <iostream>
#include <string>
#include <memory>

class PaymentStrategy {
public:
  virtual ~PaymentStrategy() = default;
  virtual void processPayment(double amount) = 0;
  virtual std::string getPaymentMethod() const = 0;
};

class CardPayment : public PaymentStrategy {
public:
  void processPayment(double amount) override {
    std::cout << "Оплата " << amount << " рублей картой" << std::endl;
  }

  std::string getPaymentMethod() const override {
    return "Карта";
  }
};

class SBPPayment : public PaymentStrategy {
public:
  void processPayment(double amount) override {
    std::cout << "Оплата " << amount << " рублей через SPB" << std::endl;
  }

  std::string getPaymentMethod() const override {
    return "SBP";
  }
};

class CashOnDeliveryPayment : public PaymentStrategy {
public:
  void processPayment(double amount) override {
    std::cout << "Оплата " << amount << " рублей наличными" << std::endl;
  }

  std::string getPaymentMethod() const override {
    return "Наличные";
  }
};

class Order {
private:
  std::unique_ptr<PaymentStrategy> paymentStrategy;
  double totalAmount;

public:
  Order(double amount) : totalAmount(amount) {}

  void setPaymentStrategy(std::unique_ptr<PaymentStrategy> strategy) {
    paymentStrategy = std::move(strategy);
  }

  void processOrder() {
    if (!paymentStrategy) {
      std::cerr << "Способ оплаты не выбран" << std::endl;
      return;
    }

    paymentStrategy->processPayment(totalAmount);
    std::cout << "Заказ оплачен через " << paymentStrategy->getPaymentMethod() << std::endl;
  }
};

/* Добавить класс и наследовать его от PaymentStrategy. Реализовать функции processPayment и getPaymentMethod.В processPayment передать цену товара.
Добавить новый способ выбора оплаты  */

int main() {
  setlocale(LC_ALL, "Russian");

  Order order(5000.0);

  order.setPaymentStrategy(std::make_unique<CardPayment>());
  order.processOrder();

  order.setPaymentStrategy(std::make_unique<SBPPayment>());
  order.processOrder();

  return 0;
}
