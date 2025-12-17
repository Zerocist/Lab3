#include <iostream>
#include <vector>
#include <memory>
#include <string>

/* Нужно написать метод, который будет проверять доступ к уведомлениям для этого наблюдателя.
Если наблюдатели назначаются системой, то за это отвечает общая система защиты сервера  */

class Observer {
public:
  virtual ~Observer() = default;
  virtual void update(const std::string& status) = 0;
};

class Order {
private:
  std::vector<Observer*> observers;
  std::string status;

public:
  void addObserver(Observer* observer) {
    observers.push_back(observer);
  }

  void removeObserver(Observer* observer) {
    observers.erase(
      std::remove(observers.begin(), observers.end(), observer),
      observers.end()
    );
  }

  void setStatus(const std::string& newStatus) {
    status = newStatus;
    notifyObservers();
  }

  std::string getStatus() const {
    return status;
  }

private:
  void notifyObservers() {
    for (auto observer : observers) {
      observer->update(status);
    }
  }
};

class ClientNotification : public Observer {
public:
  void update(const std::string& status) override {
    std::cout << "Уведомление клиенту: Статус заказа изменен на " << status << std::endl;
  }
};

class ManagerNotification : public Observer {
public:
  void update(const std::string& status) override {
    std::cout << "Уведомление менеджеру: Заказ изменил статус на " << status << std::endl;
  }
};

class AnalyticsSystem : public Observer {
public:
  void update(const std::string& status) override {
    std::cout << "Аналитика: Заказ перешел в статус " << status << std::endl;
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  Order order;

  ClientNotification client;
  ManagerNotification manager;
  AnalyticsSystem analytics;

  order.addObserver(&client);
  order.addObserver(&manager);
  order.addObserver(&analytics);

  order.setStatus("Оформлен");
  order.setStatus("В обработке");
  order.setStatus("Отправлен");

  return 0;
}
