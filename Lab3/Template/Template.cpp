#include <iostream>
#include <string>

/* Добавить функцию,проверяющую, была ли совершена предоплата и добавить новый подкласс для предоплаты если нужен новый тип именно с предоплатой  */

class OrderProcessing {
public:
  virtual ~OrderProcessing() = default;

  void processOrder(const std::string& orderId) {
    std::cout << "Обработка заказа " << orderId << std::endl;
    selectProducts();
    processPayment();
    arrangeDelivery();
    sendConfirmation();
    std::cout << "Заказ " << orderId << " обработан" << std::endl;
  }

protected:
  virtual void selectProducts() {
    std::cout << "1. Выбор товара" << std::endl;
  }

  virtual void processPayment() = 0;
  virtual void arrangeDelivery() = 0;

  virtual void sendConfirmation() {
    std::cout << "4. Отправка подтверждения заказа" << std::endl;
  }
};

class StandardOrderProcessing : public OrderProcessing {
protected:

  void processPayment() override {
    std::cout << "2. Обработка обычного платежа (оплата при получении)" << std::endl;
  }

  void arrangeDelivery() override {
    std::cout << "3. Организация стандартной доставки (5-7 дней)" << std::endl;
  }
};

class ExpressOrderProcessing : public OrderProcessing {
protected:

  void processPayment() override {
    std::cout << "2. Требуется предоплата для экспресс-доставки" << std::endl;
  }

  void arrangeDelivery() override {
    std::cout << "3. Организация экспресс-доставки (1-2 дня)" << std::endl;
  }
};

class Order {
private:
  std::unique_ptr<OrderProcessing> processor;
  std::string id;

public:
  Order(const std::string& orderId) : id(orderId) {}

  void setProcessor(std::unique_ptr<OrderProcessing> proc) {
    processor = std::move(proc);
  }

  void process() {
    if (processor) {
      processor->processOrder(id);
    }
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  Order order1("1");
  order1.setProcessor(std::make_unique<StandardOrderProcessing>());
  order1.process();

  std::cout << std::endl;

  Order order2("2");
  order2.setProcessor(std::make_unique<ExpressOrderProcessing>());
  order2.process();

  return 0;
}

