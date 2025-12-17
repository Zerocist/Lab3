#include <iostream>
#include <string>
#include <memory>
#include <vector>

/* Валидация данных,  логирование и мониторинг событий  */

class Client;
class Manager;
class Warehouse;

class Mediator {
public:
  virtual ~Mediator() = default;
  virtual void notify(std::string sender, std::string event, void* data = nullptr) = 0;
};

class Component {
protected:
  Mediator* mediator;

public:
  Component(Mediator* med = nullptr) : mediator(med) {}

  void setMediator(Mediator* med) {
    mediator = med;
  }
};

class Order {
public:
  std::string id;
  double amount;
  std::vector<std::string> items;

  Order(const std::string& _id, double _amount) : id(_id), amount(_amount) {}
};

class OrderMediator : public Mediator {
private:
  Client* client;
  Manager* manager;
  Warehouse* warehouse;

public:
  void setComponents(Client* _client, Manager* _manager, Warehouse* _warehouse) {
    client = _client;
    manager = _manager;
    warehouse = _warehouse;
  }

  void notify(std::string sender, std::string event, void* data) override {
    Order* order = static_cast<Order*>(data);

    std::cout << "Mediator: Событие '" << event
      << "' от " << sender << std::endl;

    if (event == "orderPlaced") {
      std::cout << "Менеджер уведомлен о новом заказе" << std::endl;
      std::cout << "Склад получил заявку на резервирование" << std::endl;
    }
    else if (event == "paymentConfirmed") {
      std::cout << "Склад получил указание на отгрузку" << std::endl;
    }
    else if (event == "orderShipped") {
      std::cout << "Клиент уведомлен об отправке" << std::endl;
      std::cout << "Менеджер уведомлен об отправке" << std::endl;
    }
  }
};

class Client : public Component {
public:
  void placeOrder(Order* order) {
    std::cout << "Клиент размещает заказ " << order->id << std::endl;
    if (mediator) {
      mediator->notify("Client", "orderPlaced", order);
    }
  }

  void receiveNotification(const std::string& message) {
    std::cout << "Клиент получает: " << message << std::endl;
  }
};

class Manager : public Component {
public:
  void reviewOrder(Order* order) {
    std::cout << "Менеджер проверяет заказ " << order->id << std::endl;
  }
};

class Warehouse : public Component {
public:
  void reserveItems(Order* order) {
    std::cout << "Склад резервирует товары для заказа "
      << order->id << std::endl;
  }

  void shipOrder(Order* order) {
    std::cout << "Склад отправляет заказ " << order->id << std::endl;
    if (mediator) {
      mediator->notify("Warehouse", "orderShipped", order);
    }
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  auto mediator = std::make_unique<OrderMediator>();
  auto client = std::make_unique<Client>();
  auto manager = std::make_unique<Manager>();
  auto warehouse = std::make_unique<Warehouse>();

  client->setMediator(mediator.get());
  manager->setMediator(mediator.get());
  warehouse->setMediator(mediator.get());

  auto order = std::make_unique<Order>("1", 5000.0);

  client->placeOrder(order.get());

  return 0;
}

