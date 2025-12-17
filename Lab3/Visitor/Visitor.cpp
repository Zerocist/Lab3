#include <iostream>
#include <vector>
#include <memory>

/* Создать класс для скидки и изменить оценку цены товаров  */

class Product;
class Box;

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit(Product& product) = 0;
  virtual void visit(Box& box) = 0;
};

class Element {
public:
  virtual ~Element() = default;
  virtual void accept(Visitor& visitor) = 0;
};

class Product : public Element {
private:
  std::string name;
  double price;

public:
  Product(const std::string& n, double p) : name(n), price(p) {}

  double getPrice() const { return price; }
  std::string getName() const { return name; }

  void accept(Visitor& visitor) override {
    visitor.visit(*this);
  }
};

class Box : public Element {
private:
  std::vector<std::unique_ptr<Element>> children;

public:
  void addChild(std::unique_ptr<Element> child) {
    children.push_back(std::move(child));
  }

  void accept(Visitor& visitor) override {
    visitor.visit(*this);
    for (auto& child : children) {
      child->accept(visitor);
    }
  }

  const std::vector<std::unique_ptr<Element>>& getChildren() const {
    return children;
  }
};

class DeliveryCostCalculator : public Visitor {
private:
  double totalCost;

public:
  DeliveryCostCalculator() : totalCost(0) {}

  void visit(Product& product) override {
    totalCost += product.getPrice() * 0.1;
    std::cout << "Доставка товара " << product.getName()
      << ": " << product.getPrice() * 0.1 << " рублей" << std::endl;
  }

  void visit(Box& box) override {
    totalCost += 50;
    std::cout << "Доставка коробки: 50 рублей" << std::endl;
  }

  double getTotalCost() const {
    return totalCost;
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  auto laptop = std::make_unique<Product>("Ноутбук", 50000);
  auto phone = std::make_unique<Product>("Смартфон", 30000);

  auto box = std::make_unique<Box>();
  box->addChild(std::move(laptop));
  box->addChild(std::move(phone));

  DeliveryCostCalculator deliveryCalc;
  box->accept(deliveryCalc);
  std::cout << "Общая стоимость доставки: "
    << deliveryCalc.getTotalCost() << " рублей" << std::endl;

  return 0;
}

