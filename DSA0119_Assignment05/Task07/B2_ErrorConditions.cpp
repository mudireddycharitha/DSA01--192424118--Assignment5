
#include <iostream>
#include <stdexcept>
#include <string>
#include <new>
#include <limits>
using namespace std;

class ClaimError : public runtime_error {
public:
    using runtime_error::runtime_error;
};

class UnknownPackageError : public runtime_error {
public:
    using runtime_error::runtime_error;
};

class TestResource {
private:
    string name;

public:
    TestResource(const string& resourceName)
        : name(resourceName) {
        cout << name << " created\n";
    }

    ~TestResource() {
        cout << name << " destroyed\n";
    }
};

double validateCharge(double amount) {
    if (amount < 0) {
        throw invalid_argument("negative charge");
    }

    return amount;
}

double approveClaim(double billed, double cover) {
    if (billed < 0 || cover < 0) {
        throw invalid_argument("negative amount");
    }

    if (billed > cover) {
        throw ClaimError("claim exceeds cover");
    }

    return billed;
}

double getPackageRate(const string& code) {
    if (code == "OP") {
        return 5000;
    }

    if (code == "IP") {
        return 20000;
    }

    if (code == "DC") {
        return 12000;
    }

    throw UnknownPackageError("unknown package code");
}

double readAmount(const string& input) {
    size_t position = 0;

    double value = stod(input, &position);

    if (position != input.length()) {
        throw invalid_argument("invalid numeric text");
    }

    return value;
}

template <typename Function>
void runTest(const string& title, Function operation) {
    cout << "\n--- " << title << " ---\n";

    try {
        TestResource resource(title);

        operation();

        cout << "Test completed\n";
    }
    catch (const bad_alloc& e) {
        cout << "Memory error: " << e.what() << '\n';
    }
    catch (const ClaimError& e) {
        cout << "Claim error: " << e.what() << '\n';
    }
    catch (const UnknownPackageError& e) {
        cout << "Package error: " << e.what() << '\n';
    }
    catch (const exception& e) {
        cout << "Standard error: " << e.what() << '\n';
    }
    catch (...) {
        cout << "Unknown error caught\n";
    }

    cout << "Program continues\n";
}

int main() {
    cout << "HOSPITAL BILLING - ERROR TESTS\n";

    runTest("Negative Charge", []() {
        validateCharge(-500);
    });

    runTest("Excess Claim", []() {
        approveClaim(60000, 50000);
    });

    runTest("Unknown Package", []() {
        cout << "Package rate: "
             << getPackageRate("XYZ") << '\n';
    });

    runTest("Invalid Numeric Input", []() {
        double amount = readAmount("abc");
        cout << "Amount: " << amount << '\n';
    });

    runTest("Memory Allocation Failure", []() {
        cout << "Requesting extremely large memory...\n";

        volatile size_t request =
            numeric_limits<size_t>::max();

        char* memory = new char[request];

        delete[] memory;
    });

    runTest("Catch-All Handler", []() {
        throw 100;
    });

    cout << "\nAll error tests finished.\n";

    return 0;
}
