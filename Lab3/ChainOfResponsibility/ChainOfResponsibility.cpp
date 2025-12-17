#include <iostream>
#include <memory>
#include <string>
#include <vector>

/* Добавить в RefundResponse после if отправку данных от, условно, системы
return { false, "System", "Запрос не может быть обработан" }; 
Это обработает запрос, если он никуда не отправится   */

struct Handler {
  double amount;
  std::string reason;
  int days;

  Handler(double _amount, const std::string& _reason, int _days)
    : amount(_amount), reason(_reason), days(_days) {
  }
};

struct RefundResponse {
  bool processed;
  std::string handler;
  std::string message;
};

class RefundHandler {
protected:
  std::unique_ptr<RefundHandler> nextHandler;

public:
  virtual ~RefundHandler() = default;

  void setNext(std::unique_ptr<RefundHandler> handler) {
    nextHandler = std::move(handler);
  }

  virtual RefundResponse handleRequest(const Handler& request) {
    if (nextHandler) {
      return nextHandler->handleRequest(request);
    }
  }

  virtual bool canHandle(const Handler& request) = 0;
};

class ManagerHandler : public RefundHandler {
private:
  const double MAX_AMOUNT = 5000.0;
  const int MAX_DAYS = 14;

public:
  bool canHandle(const Handler& request) override {
    return request.amount <= MAX_AMOUNT &&
      request.days <= MAX_DAYS;
  }

  RefundResponse handleRequest(const Handler& request) override {
    if (canHandle(request)) {
      return {
          true,
          "Manager",
          "Возврат на сумму " + std::to_string(request.amount) +
          " одобрен менеджером"
      };
    }

    return RefundHandler::handleRequest(request);
  }
};

class SupervisorHandler : public RefundHandler {
private:
  const double MAX_AMOUNT = 20000.0;
  const int MAX_DAYS = 30;

public:
  bool canHandle(const Handler& request) override {
    return request.amount <= MAX_AMOUNT &&
      request.days <= MAX_DAYS;
  }

  RefundResponse handleRequest(const Handler& request) override {
    if (canHandle(request)) {
      return {
          true,
          "Supervisor",
          "Возврат на сумму " + std::to_string(request.amount) +
          " одобрен руководителем"
      };
    }

    return RefundHandler::handleRequest(request);
  }
};

class SupportHandler : public RefundHandler {
public:
  bool canHandle(const Handler& request) override {
    return true;
  }

  RefundResponse handleRequest(const Handler& request) override {
    return {
        false,
        "Support",
        "Запрос передан в службу поддержки для консультации. " +
        std::string("Свяжитесь с нами по телефону.")
    };
  }
};

class RefundSystem {
private:
  std::unique_ptr<RefundHandler> chain;

public:
  RefundSystem() {
    auto manager = std::make_unique<ManagerHandler>();
    auto supervisor = std::make_unique<SupervisorHandler>();
    auto support = std::make_unique<SupportHandler>();

    manager->setNext(std::move(supervisor));
    manager->setNext(std::move(support));

    chain = std::move(manager);
  }

  RefundResponse processRefund(const Handler& request) {
    std::cout << "Обработка запроса на возврат: "
      << request.amount << " руб., "
      << request.days << " дней" << std::endl;

    return chain->handleRequest(request);
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  RefundSystem system;

  std::vector<Handler> requests = {
      {3000.0, "Не тот размер", 5},
      {10000.0, "Сломан", 20},
      {50000.0, "Не тот цвет", 2},
      {1000.0, "Другое", 50}
  };

  for (const auto& request : requests) {
    auto response = system.processRefund(request);

    std::cout << "Результат: " << response.message
      << " (Обработчик: " << response.handler << ")" << std::endl;
  }

  return 0;
}
