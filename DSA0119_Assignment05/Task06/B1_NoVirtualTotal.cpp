
#include <iostream>
#include <string>

using namespace std;

class Bill {
public:
    double total() const = 0;

    virtual ~Bill() {}
};

class OutPatient : public Bill {
public:
    double total() const {
        return 1700;
    }
};

int main() {
    Bill* b = new OutPatient();

    cout << b->total() << '\n';

    delete b;

    return 0;
}
