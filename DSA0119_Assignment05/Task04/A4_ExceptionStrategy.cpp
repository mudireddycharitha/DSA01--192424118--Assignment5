
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

using namespace std;

class BillingError : public runtime_error {
public:
    BillingError(const string& message)
        : runtime_error(message) {}
};

class NegativeChargeError : public BillingError {
public:
    NegativeChargeError()
        : BillingError("Negative charges are not allowed.") {}
};

class ClaimError : public BillingError {
public:
    ClaimError(const string& message)
        : BillingError(message) {}
};

class CoverLimitError : public ClaimError {
public:
    CoverLimitError()
        : ClaimError("Insurance claim exceeds cover limit.") {}
};

class UnknownPackageError : public BillingError {
public:
    UnknownPackageError()
        : BillingError("Unknown government package.") {}
};

class MissingPatientError : public BillingError {
public:
    MissingPatientError()
        : BillingError("Patient record not found.") {}
};

class HospitalBilling {
private:
    double charge;

public:
    HospitalBilling() noexcept : charge(0) {}

    ~HospitalBilling() noexcept {
    }

    double getCharge() const noexcept {
        return charge;
    }

    void setCharges(double amount) {
        if (amount < 0)
            throw NegativeChargeError();

        charge = amount;
    }

    void validateClaim(double claim, double limit) const {
        if (claim > limit)
            throw CoverLimitError();
    }

    void validatePackage(const string& package) const {
        if (package != "OUTPATIENT" &&
            package != "INPATIENT") {
            throw UnknownPackageError();
        }
    }

    void findPatient(int patientId) const {
        if (patientId != 101)
            throw MissingPatientError();
    }
};

int main() {

    HospitalBilling bill;

    cout << "HOSPITAL EXCEPTION HANDLING\n";
    cout << "----------------------------\n";

    try {
        bill.setCharges(-500);
    }
    catch (const NegativeChargeError& e) {
        cout << "Test 1: " << e.what() << '\n';
    }

    try {
        bill.validateClaim(50000, 30000);
    }
    catch (const CoverLimitError& e) {
        cout << "Test 2: " << e.what() << '\n';
    }

    try {
        bill.validatePackage("UNKNOWN");
    }
    catch (const UnknownPackageError& e) {
        cout << "Test 3: " << e.what() << '\n';
    }

    try {
        bill.findPatient(999);
    }
    catch (const MissingPatientError& e) {
        cout << "Test 4: " << e.what() << '\n';
    }

    try {
        bill.setCharges(5000);
        cout << "Test 5: Valid charge = Rs. "
             << bill.getCharge() << '\n';
    }
    catch (const BillingError& e) {
        cout << "Billing Error: " << e.what() << '\n';
    }

    try {
        unique_ptr<int> data(new int[100]);
        cout << "Test 6: Memory allocated successfully.\n";
    }
    catch (const bad_alloc& e) {
        cout << "Test 6: Memory allocation failed: "
             << e.what() << '\n';
    }

    cout << "Exception testing completed.\n";

    return 0;
}
