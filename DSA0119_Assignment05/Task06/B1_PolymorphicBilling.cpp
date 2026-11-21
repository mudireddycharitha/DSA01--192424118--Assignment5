
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <map>
#include <stdexcept>

using namespace std;

// ======================================
// ABSTRACT BILL CLASS
// ======================================

class Bill {
protected:
    string patient;

public:
    explicit Bill(const string& p) : patient(p) {}

    virtual ~Bill() {
        cout << "Destroying Bill for " << patient << '\n';
    }

    virtual double total() const = 0;

    virtual string type() const {
        return "Bill";
    }

    void print() const {
        cout << type() << " " << patient
             << ": " << total() << '\n';
    }
};

// ======================================
// OUTPATIENT BILL
// ======================================

class OutPatient : public Bill {
private:
    double consultation;
    double tests;

public:
    OutPatient(const string& p, double c, double t)
        : Bill(p), consultation(c), tests(t) {}

    ~OutPatient() override {
        cout << "Destroying OutPatient\n";
    }

    double total() const override {
        return consultation + tests;
    }

    string type() const override {
        return "OP";
    }
};

// ======================================
// INPATIENT BILL
// ======================================

class InPatient : public Bill {
private:
    int days;
    double perDay;
    double surgery;

public:
    InPatient(const string& p, int d, double rate, double s)
        : Bill(p), days(d), perDay(rate), surgery(s) {}

    ~InPatient() override {
        cout << "Destroying InPatient\n";
    }

    double total() const override {
        return days * perDay + surgery;
    }

    string type() const override {
        return "IP";
    }
};

// ======================================
// DAYCARE BILL
// ======================================

class DayCare : public Bill {
private:
    double packagePrice;

public:
    DayCare(const string& p, double price)
        : Bill(p), packagePrice(price) {}

    double total() const override {
        return packagePrice;
    }

    string type() const override {
        return "DayCare";
    }
};

// ======================================
// EMERGENCY BILL
// ======================================

class Emergency : public Bill {
private:
    double baseCharge;
    int hours;
    double hourlyRate;

public:
    Emergency(const string& p, double base,
              int h, double rate)
        : Bill(p), baseCharge(base),
          hours(h), hourlyRate(rate) {}

    double total() const override {
        return baseCharge + hours * hourlyRate;
    }

    string type() const override {
        return "Emergency";
    }
};

// ======================================
// ABSTRACT PAYER CLASS
// ======================================

class Payer {
public:
    virtual double amountCovered(const Bill& bill) const = 0;

    virtual string payerName() const = 0;

    virtual ~Payer() {}
};

// ======================================
// SELF PAY
// ======================================

class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override {
        return 0;
    }

    string payerName() const override {
        return "SelfPay";
    }
};

// ======================================
// INSURANCE
// ======================================

class Insurance : public Payer {
private:
    double coverLimit;
    double copayment;

public:
    Insurance(double limit, double copay)
        : coverLimit(limit), copayment(copay) {

        if (limit < 0 || copay < 0 || copay > 100)
            throw invalid_argument("Invalid insurance settings");
    }

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverLimit);

        return eligible * (100 - copayment) / 100.0;
    }

    string payerName() const override {
        return "Insurance";
    }
};

// ======================================
// GOVERNMENT SCHEME
// ======================================

class GovernmentScheme : public Payer {
private:
    map<string, double> packageRates;

public:
    GovernmentScheme() {
        packageRates["OP"] = 5000;
        packageRates["IP"] = 20000;
        packageRates["DayCare"] = 12000;
        packageRates["Emergency"] = 15000;
    }

    double amountCovered(const Bill& bill) const override {
        map<string, double>::const_iterator it =
            packageRates.find(bill.type());

        if (it == packageRates.end())
            throw invalid_argument("Unknown package");

        return min(bill.total(), it->second);
    }

    string payerName() const override {
        return "Government";
    }
};

// ======================================
// CORPORATE PAYER
// ======================================

class CorporatePayer : public Payer {
private:
    double coverLimit;
    double coveragePercentage;

public:
    CorporatePayer(double limit, double percentage)
        : coverLimit(limit), coveragePercentage(percentage) {

        if (limit < 0 || percentage < 0 || percentage > 100)
            throw invalid_argument("Invalid corporate settings");
    }

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverLimit);

        return eligible * coveragePercentage / 100.0;
    }

    string payerName() const override {
        return "Corporate";
    }
};

// ======================================
// BILLING PROCESSING
// ======================================

int main() {
    try {
        cout << fixed << setprecision(2);

        vector<unique_ptr<Bill> > bills;

        bills.push_back(
            unique_ptr<Bill>(new OutPatient("Ravi", 500, 1200))
        );

        bills.push_back(
            unique_ptr<Bill>(new InPatient("Meena", 3, 2500, 40000))
        );

        bills.push_back(
            unique_ptr<Bill>(new DayCare("Arun", 9000))
        );

        bills.push_back(
            unique_ptr<Bill>(new Emergency("Priya", 3000, 4, 2000))
        );

        vector<unique_ptr<Payer> > payers;

        payers.push_back(unique_ptr<Payer>(new SelfPay));

        payers.push_back(
            unique_ptr<Payer>(new Insurance(30000, 20))
        );

        payers.push_back(
            unique_ptr<Payer>(new GovernmentScheme)
        );

        payers.push_back(
            unique_ptr<Payer>(new CorporatePayer(25000, 80))
        );

        cout << "\nHOSPITAL POLYMORPHIC BILLING SYSTEM\n";
        cout << "====================================\n";

        cout << "\nBILL DETAILS\n";

        double grandTotal = 0;

        for (size_t i = 0; i < bills.size(); i++) {
            bills[i]->print();
            grandTotal += bills[i]->total();
        }

        cout << "\nTotal Hospital Billing: Rs. "
             << grandTotal << '\n';

        cout << "\nPAYER COVERAGE VALIDATION\n";

        cout << left << setw(15) << "Bill Type"
             << setw(15) << "Payer"
             << right << setw(14) << "Bill Total"
             << setw(14) << "Covered"
             << setw(14) << "Patient Pays"
             << '\n';

        cout << string(72, '-') << '\n';

        for (size_t i = 0; i < bills.size(); i++) {

            for (size_t j = 0; j < payers.size(); j++) {

                double amount = bills[i]->total();

                double covered =
                    payers[j]->amountCovered(*bills[i]);

                double patientPays = amount - covered;

                cout << left << setw(15) << bills[i]->type()
                     << setw(15) << payers[j]->payerName()
                     << right << setw(14) << amount
                     << setw(14) << covered
                     << setw(14) << patientPays
                     << '\n';
            }

            cout << '\n';
        }

        cout << "All 16 bill-payer combinations tested.\n";

        cout << "\nTotal Bills Tested: " << bills.size();
        cout << "\nTotal Payer Types: " << payers.size() << '\n';
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << '\n';
    }

    cout << "\nBilling processing completed.\n";

    return 0;
}
