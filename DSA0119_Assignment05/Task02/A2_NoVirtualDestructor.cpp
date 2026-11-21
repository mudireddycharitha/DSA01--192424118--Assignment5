
#include <iostream>
using namespace std;

class Bill {
public:
    ~Bill() {
        cout << "~Bill ";
    }

    virtual double total() const = 0;

    void show() const {
        cout << total() << ' ';
    }
};

class OPBill : public Bill {
public:
    ~OPBill() {
        cout << "~OPBill ";
    }

    double total() const override {
        return 500;
    }
};

int main() {
    Bill* b = new OPBill;

    b->show();

    delete b;

    cout << '\n';

    return 0;
}
