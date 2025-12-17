#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <memory>

/* Сохранение стека или вектора. Потом указываем количество шагов на которое переместить.
Ограничение по памяти, ведь создается полная копия обьектов.  */

class Memento {
private:
  std::vector<std::string> items;

public:
  Memento(const std::vector<std::string>& itemsList)
    : items(itemsList) {
  }

  std::vector<std::string> getItems() const {
    return items;
  }
};

class ShoppingCart {
private:
  std::vector<std::string> items;

public:
  void addItem(const std::string& item) {
    items.push_back(item);
    std::cout << "Добавлен товар: " << item << std::endl;
  }

  void removeItem(const std::string& item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
      items.erase(it);
      std::cout << "Удален товар: " << item << std::endl;
    }
  }

  std::unique_ptr<Memento> save() const {
    std::cout << "Сохранение состояния корзины ("
      << items.size() << " товаров)";
    return std::make_unique<Memento>(items);
  }

  void restore(const Memento* memento) {
    if (memento) {
      items = memento->getItems();
      std::cout << "Восстановление состояния корзины ("
        << items.size() << " товаров)" << std::endl;
    }
  }

  void display() const {
    std::cout << "Товары в корзине (" << items.size() << "): " << std::endl;
    for (const auto& item : items) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
};

class Caretaker {
private:
  std::stack<std::unique_ptr<Memento>> history;
  ShoppingCart& cart;

public:
  Caretaker(ShoppingCart& c) : cart(c) {}

  void backup() {
    history.push(cart.save());
  }

  void undo() {
    if (history.empty()) {
      std::cout << "История пуста" << std::endl;
      return;
    }

    history.pop();

    if (history.empty()) {
      cart.restore(nullptr);
    }
    else {
      cart.restore(history.top().get());
    }
  }

  void undoMultiple(int steps) {
    if (steps <= 0) return;

    for (int i = 0; i < steps && !history.empty(); i++) {
      history.pop();
    }

    if (history.empty()) {
      cart.restore(nullptr);
    }
    else {
      cart.restore(history.top().get());
    }

    std::cout << "Отменено " << steps << " действий" << std::endl;
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  ShoppingCart cart;
  Caretaker caretaker(cart);

  caretaker.backup();
  cart.addItem("Ноутбук");
  cart.display();

  caretaker.backup();
  cart.addItem("Смартфон");
  cart.display();

  caretaker.backup();
  cart.addItem("Наушники");
  cart.display();

  std::cout << std::endl << "Отмена одного действия:" << std::endl;
  caretaker.undo();
  cart.display();

  return 0;
}


