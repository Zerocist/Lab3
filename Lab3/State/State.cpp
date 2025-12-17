#include <iostream>
#include <memory>
#include <string>

/* Проверять следующее состояние и если оно совпадает с нужным, переходить  */

class OrderContext;

class OrderState {
public:
  virtual ~OrderState() = default;
  virtual void process(OrderContext& context) = 0;
  virtual std::string getStatus() const = 0;
  virtual bool canTransitionTo(const std::string& nextState) const = 0;
};

class OrderContext {
private:
  std::unique_ptr<OrderState> currentState;

public:
  OrderContext(std::unique_ptr<OrderState> initialState);

  void transitionTo(std::unique_ptr<OrderState> newState);
  void process();
  std::string getCurrentStatus() const;
};

class NewOrderState : public OrderState {
public:
  void process(OrderContext& context) override;
  std::string getStatus() const override;
  bool canTransitionTo(const std::string& nextState) const override;
};

class ProcessingOrderState : public OrderState {
public:
  void process(OrderContext& context) override;
  std::string getStatus() const override;
  bool canTransitionTo(const std::string& nextState) const override;
};

class ShippedOrderState : public OrderState {
public:
  void process(OrderContext& context) override;
  std::string getStatus() const override;
  bool canTransitionTo(const std::string& nextState) const override;
};

class DeliveredOrderState : public OrderState {
public:
  void process(OrderContext& context) override;
  std::string getStatus() const override;
  bool canTransitionTo(const std::string& nextState) const override;
};

OrderContext::OrderContext(std::unique_ptr<OrderState> initialState)
  : currentState(std::move(initialState)) {
}

void OrderContext::transitionTo(std::unique_ptr<OrderState> newState) {
  if (currentState && !currentState->canTransitionTo(newState->getStatus())) {
    std::cerr << "Недопустимый переход из "
      << currentState->getStatus()
      << " в " << newState->getStatus() << std::endl;
    return;
  }

  currentState = std::move(newState);
  std::cout << "Состояние изменено на: " << currentState->getStatus() << std::endl;
}

void OrderContext::process() {
  if (currentState) {
    currentState->process(*this);
  }
}

std::string OrderContext::getCurrentStatus() const {
  return currentState ? currentState->getStatus() : "Неизвестно";
}

void NewOrderState::process(OrderContext& context) {
  std::cout << "Обработка нового заказа" << std::endl;
}

std::string NewOrderState::getStatus() const {
  return "Новый";
}

bool NewOrderState::canTransitionTo(const std::string& nextState) const {
  return nextState == "В обработке" || nextState == "Отменен";
}

void ProcessingOrderState::process(OrderContext& context) {
  std::cout << "Заказ в обработке" << std::endl;
}

std::string ProcessingOrderState::getStatus() const {
  return "В обработке";
}

bool ProcessingOrderState::canTransitionTo(const std::string& nextState) const {
  return nextState == "Отправлен" || nextState == "Отменен";
}

void ShippedOrderState::process(OrderContext& context) {
  std::cout << "Заказ отправлен" << std::endl;
}

std::string ShippedOrderState::getStatus() const {
  return "Отправлен";
}

bool ShippedOrderState::canTransitionTo(const std::string& nextState) const {
  return nextState == "Доставлен";
}

void DeliveredOrderState::process(OrderContext& context) {
  std::cout << "Заказ уже доставлен. Дальнейшая обработка невозможна." << std::endl;
}

std::string DeliveredOrderState::getStatus() const {
  return "Доставлен";
}

bool DeliveredOrderState::canTransitionTo(const std::string& nextState) const {
  return false;
}

int main() {
  setlocale(LC_ALL, "Russian");

  OrderContext order(std::make_unique<NewOrderState>());

  order.process();
  order.transitionTo(std::make_unique<ProcessingOrderState>());

  order.process();
  order.transitionTo(std::make_unique<ShippedOrderState>());

  order.process();

  order.transitionTo(std::make_unique<NewOrderState>());

  return 0;
}

