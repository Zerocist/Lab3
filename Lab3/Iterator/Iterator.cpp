#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

/* Через if добавить проверку количество найденных товаров в категории. Если их будет 0 то просто оставить 
пустой или выводить сообщение что товаров не найдено  */

class Product {
public:
  std::string name;
  double price;
  std::string category;
  int popularity;

  Product(const std::string& _name, double _price, const std::string& _category, int _population)
    : name(_name), price(_price), category(_category), popularity(_population) {
  }
};

class CatalogIterator {
public:
  virtual ~CatalogIterator() = default;
  virtual bool hasNext() const = 0;
  virtual Product Next() = 0;
  virtual void Reset() = 0;
};

class CategoryIterator : public CatalogIterator {
private:
  std::vector<Product> products;
  std::string category;
  size_t current;

public:
  CategoryIterator(const std::vector<Product>& _allProducts, const std::string& _category)
    : category(_category), current(0) {
    for (const auto& product : _allProducts) {
      if (product.category == _category) {
        products.push_back(product);
      }
    }
  }

  bool hasNext() const override {
    return current < products.size();
  }

  Product Next() override {
    if (!hasNext()) {
      throw std::runtime_error("No more products");
    }
    return products[current++];
  }

  void Reset() override {
    current = 0;
  }
};

class PriceIterator : public CatalogIterator {
private:
  std::vector<Product> products;
  size_t current;

public:
  PriceIterator(const std::vector<Product>& _allProducts) : current(0) {
    products = _allProducts;
    std::sort(products.begin(), products.end(),
      [](const Product& a, const Product& b) {
        return a.price < b.price;
      });
  }

  bool hasNext() const override {
    return current < products.size();
  }

  Product Next() override {
    if (!hasNext()) {
      throw std::runtime_error("No more products");
    }
    return products[current++];
  }

  void Reset() override {
    current = 0;
  }
};

class Catalog {
private:
  std::vector<Product> products;
  std::unique_ptr<CatalogIterator> iterator;

public:
  void addProduct(const Product& _product) {
    products.push_back(_product);
  }

  void setIterator(std::unique_ptr<CatalogIterator> _iterator) {
    iterator = std::move(_iterator);
  }

  void displayProducts() {
    if (!iterator) {
      std::cout << "Итератор не установлен" << std::endl;
      return;
    }

    iterator->Reset();
    int count = 0;
    while (iterator->hasNext()) {
      Product product = iterator->Next();
      std::cout << ++count << ". " << product.name
        << " - " << product.price
        << " рублей (" << product.category << ")" << std::endl;
    }

  }

  std::vector<Product> getProgucts() {
    return products;
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  Catalog catalog;

  catalog.addProduct(Product("Кровать", 20000, "Мебель", 1));
  catalog.addProduct(Product("Смартфон", 30000, "Электроника", 8));
  catalog.addProduct(Product("RED Bull", 350, "Энергетик", 15));
  catalog.addProduct(Product("Наушники", 2000, "Электроника", 6));

  std::cout << "Товары в категории \"Электроника\":" << std::endl;
  catalog.setIterator(std::make_unique<CategoryIterator>(
    catalog.getProgucts(),
    "Электроника"
  ));
  catalog.displayProducts();

  std::cout << std::endl << "Товары по цене (по возрастанию):" << std::endl;
  catalog.setIterator(std::make_unique<PriceIterator>(
    catalog.getProgucts()
  ));
  catalog.displayProducts();

  return 0;
}
