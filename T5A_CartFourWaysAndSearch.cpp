#include <iostream>
#include <string>
#include <iomanip>

class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0.0;

public:
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) sum += prices[i];
        return sum * (100.0 - discount) / 100.0;
    }
};

Cart* findHighestCart(Cart** carts, int count) {
    if (count <= 0) return nullptr;
    Cart* maxCart = *carts;
    for (int i = 1; i < count; ++i) {
        if ((*(carts + i))->total() > maxCart->total()) {
            maxCart = *(carts + i);
        }
    }
    return maxCart;
}

int main() {
    Cart c1;
    c1.add("Apples", 100).setDiscount(10);

    Cart* ptr = &c1;
    ptr->add("Bread", 50);

    Cart& ref = c1;
    ref.add("Milk", 60);

    Cart* dynCart = new Cart();
    dynCart->add("Laptop Bag", 1200).setDiscount(5);

    Cart c3;
    c3.add("Coffee", 350);

    Cart* activeCarts[3] = { &c1, dynCart, &c3 };
    Cart* best = findHighestCart(activeCarts, 3);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Cart 1 Total: " << c1.total() << '\n';
    std::cout << "Dynamic Cart Total: " << dynCart->total() << '\n';
    std::cout << "Cart 3 Total: " << c3.total() << '\n';
    std::cout << "Highest Cart Total: " << best->total() << '\n';

    delete dynCart;
    return 0;
}
