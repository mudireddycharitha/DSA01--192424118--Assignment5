
#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <chrono>
#include <stdexcept>
#include <iomanip>
using namespace std;
using namespace chrono;

struct BillRecord {
    int type;
    double first;
    double second;
};

class Bill {
public:
    virtual double total() const = 0;
    virtual ~Bill() {}
};

class OutPatientBill : public Bill {
    double consultation, medicine;

public:
    OutPatientBill(double c, double m)
        : consultation(c), medicine(m) {}

    double total() const override {
        return consultation + medicine;
    }
};

class InPatientBill : public Bill {
    double days, dailyRate, surgery;

public:
    InPatientBill(double d, double r, double s)
        : days(d), dailyRate(r), surgery(s) {}

    double total() const override {
        return days * dailyRate + surgery;
    }
};

class DayCareBill : public Bill {
    double procedure, medicine;

public:
    DayCareBill(double p, double m)
        : procedure(p), medicine(m) {}

    double total() const override {
        return procedure + medicine;
    }
};

class EmergencyBill : public Bill {
    double emergencyFee, hours, hourlyRate;

public:
    EmergencyBill(double e, double h, double r)
        : emergencyFee(e), hours(h), hourlyRate(r) {}

    double total() const override {
        return emergencyFee + hours * hourlyRate;
    }
};

double switchTotal(const BillRecord& b) {
    switch (b.type) {
        case 0:
            return b.first + b.second;

        case 1:
            return b.first * b.second + 40000;

        case 2:
            return b.first + b.second;

        case 3:
            return b.first + b.second;

        default:
            return 0;
    }
}
struct OPVariant {
    double consultation, medicine;

    double total() const {
        return consultation + medicine;
    }
};

struct IPVariant {
    double days, dailyRate, surgery;

    double total() const {
        return days * dailyRate + surgery;
    }
};

struct DCVariant {
    double procedure, medicine;

    double total() const {
        return procedure + medicine;
    }
};

struct EmergencyVariant {
    double fee, hours, hourlyRate;

    double total() const {
        return fee + hours * hourlyRate;
    }
};

using BillVariant = variant<
    OPVariant,
    IPVariant,
    DCVariant,
    EmergencyVariant
>;

double variantTotal(const BillVariant& b) {
    return visit(
        [](const auto& bill) {
            return bill.total();
        },
        b
    );
}

template <typename Function>
double measureTime(Function operation) {
    auto start = high_resolution_clock::now();

    volatile double result = operation();

    auto finish = high_resolution_clock::now();

    (void)result;

    return duration<double, milli>(finish - start).count();
}

void correctnessTest() {
    cout << "\nB3.1 - CORRECTNESS TEST\n";

    OutPatientBill op(500, 1200);
    InPatientBill ip(3, 2500, 40000);
    DayCareBill dc(6000, 3000);
    EmergencyBill em(3000, 4, 2000);

    Bill* virtualBills[] = {
        &op, &ip, &dc, &em,
        &op, &ip, &dc, &em,
        &op, &ip
    };

    BillRecord switchBills[] = {
        {0, 500, 1200},
        {1, 3, 2500},
        {2, 6000, 3000},
        {3, 3000, 8000},
        {0, 500, 1200},
        {1, 3, 2500},
        {2, 6000, 3000},
        {3, 3000, 8000},
        {0, 500, 1200},
        {1, 3, 2500}
    };

    double virtualSum = 0;
    double switchSum = 0;

    for (int i = 0; i < 10; i++) {
        virtualSum += virtualBills[i]->total();
        switchSum += switchTotal(switchBills[i]);
    }

    cout << fixed << setprecision(2);

    cout << "Virtual total: Rs. "
         << virtualSum << '\n';

    cout << "Switch total : Rs. "
         << switchSum << '\n';

    if (virtualSum == switchSum) {
        cout << "RESULT: Both implementations agree.\n";
    }
    else {
        cout << "RESULT: Totals do not match.\n";
    }
}

void benchmarkBills(int n) {
    cout << "\nBill count: " << n << '\n';

    static OutPatientBill op(500, 1200);
    static InPatientBill ip(3, 2500, 40000);
    static DayCareBill dc(6000, 3000);
    static EmergencyBill em(3000, 4, 2000);

    Bill* objects[] = {&op, &ip, &dc, &em};

    vector<Bill*> virtualBills;
    vector<BillRecord> switchBills;

    virtualBills.reserve(n);
    switchBills.reserve(n);

    for (int i = 0; i < n; i++) {
        int type = i % 4;

        virtualBills.push_back(objects[type]);

        if (type == 0)
            switchBills.push_back({0, 500, 1200});
        else if (type == 1)
            switchBills.push_back({1, 3, 2500});
        else if (type == 2)
            switchBills.push_back({2, 6000, 3000});
        else
            switchBills.push_back({3, 3000, 8000});
    }

    double virtualTime = measureTime([&]() {
        double sum = 0;

        for (Bill* b : virtualBills)
            sum += b->total();

        return sum;
    });

    double switchTime = measureTime([&]() {
        double sum = 0;

        for (const BillRecord& b : switchBills)
            sum += switchTotal(b);

        return sum;
    });

    cout << "Virtual function time: "
         << virtualTime << " ms\n";

    cout << "Switch statement time: "
         << switchTime << " ms\n";
}

bool validateReturnCode(double amount) {
    return amount >= 0;
}

bool validateException(double amount) {
    try {
        if (amount < 0)
            throw invalid_argument("negative amount");

        return true;
    }
    catch (const invalid_argument&) {
        return false;
    }
}

void benchmarkExceptions(int invalidPercent) {
    const int N = 1000000;

    vector<double> claims(N, 1000.0);

    int invalidCount = N * invalidPercent / 100;

    for (int i = 0; i < invalidCount; i++)
        claims[i] = -100.0;

    cout << "\nInvalid percentage: "
         << invalidPercent << "%\n";

    double returnTime = measureTime([&]() {
        int accepted = 0;

        for (double amount : claims) {
            if (validateReturnCode(amount))
                accepted++;
        }

        return static_cast<double>(accepted);
    });

    double exceptionTime = measureTime([&]() {
        int accepted = 0;

        for (double amount : claims) {
            if (validateException(amount))
                accepted++;
        }

        return static_cast<double>(accepted);
    });

    cout << "Return-code time: "
         << returnTime << " ms\n";

    cout << "Exception time  : "
         << exceptionTime << " ms\n";
}

void benchmarkVariant() {
    const int N = 10000000;

    cout << "\nVARIANT BENCHMARK\n";
    cout << "Bill count: " << N << '\n';

    vector<BillVariant> bills;
    bills.reserve(N);

    for (int i = 0; i < N; i++) {
        switch (i % 4) {
            case 0:
                bills.emplace_back(OPVariant{500, 1200});
                break;

            case 1:
                bills.emplace_back(IPVariant{3, 2500, 40000});
                break;

            case 2:
                bills.emplace_back(DCVariant{6000, 3000});
                break;

            case 3:
                bills.emplace_back(
                    EmergencyVariant{3000, 4, 2000}
                );
                break;
        }
    }

    double variantTime = measureTime([&]() {
        double sum = 0;

        for (const BillVariant& b : bills)
            sum += variantTotal(b);

        return sum;
    });

    cout << "Variant time: "
         << variantTime << " ms\n";
}

int main() {
    cout << "B3 - PERFORMANCE COMPARISON\n";

    correctnessTest();

    cout << "\nB3.2 - VIRTUAL VS SWITCH\n";

    benchmarkBills(1000);
    benchmarkBills(1000000);
    benchmarkBills(10000000);

    cout << "\nB3.3 - COST OF EXCEPTIONS\n";

    benchmarkExceptions(0);
    benchmarkExceptions(1);
    benchmarkExceptions(50);

    benchmarkVariant();

    cout << "\nALL BENCHMARKS COMPLETED\n";

    return 0;
}
