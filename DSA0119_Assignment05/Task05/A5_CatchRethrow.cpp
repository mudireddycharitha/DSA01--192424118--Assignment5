
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

class BillingError : public runtime_error {
public:
    BillingError(const string& message)
        : runtime_error(message) {}
};

void performBilling() {
    unique_ptr<int> billData(new int(5000));

    cout << "Billing started.\n";

    if (*billData > 0) {
        throw BillingError("Invalid insurance claim.");
    }

    cout << "Billing completed.\n";
}

void billingService() {
    try {
        performBilling();
    }
    catch (const BillingError& e) {
        cout << "Service Log: " << e.what() << '\n';

        throw;
    }
}

int main() {
    cout << "HOSPITAL BILLING SYSTEM\n";
    cout << "-----------------------\n";

    try {
        billingService();
    }
    catch (const BillingError& e) {
        cout << "User Message: "
             << e.what() << '\n';
    }

    cout << "Program continues safely.\n";

    return 0;
}
