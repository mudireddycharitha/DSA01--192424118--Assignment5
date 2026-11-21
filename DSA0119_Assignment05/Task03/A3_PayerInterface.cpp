#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
#include <stdexcept>

using namespace std;

class Bill {
public:
    virtual double total() const = 0;
    virtual string type() const = 0;

    virtual ~Bill() {}
};

class OutPatientBill : public Bill {
private:
    double consultationFee;
    double testCharges;

public:
    OutPatientBill(double consultation, double tests) {
        if (consultation < 0 || tests < 0)
            throw invalid_argument("Negative bill charges");

        consultationFee = consultation;
        testCharges = tests;
    }

    double total() const override {
        return consultationFee + testCharges;
    }

    string type() const override {
        return "OutPatient";
    }
};

class Payer {
public:
    virtual double amountCovered(const Bill& bill) const = 0;

    virtual string payerName() const = 0;

    virtual ~Payer() {}
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& bill) const override {
        return 0.0;
    }

    string payerName() const override {
        return "SelfPay";
    }
};

class Insurance : public Payer {
private:
    double coverLimit;
    double copaymentPercentage;

public:
    Insurance(double limit, double copay) {
        if (limit < 0 || copay < 0 || copay > 100)
            throw invalid_argument("Invalid insurance details");

        coverLimit = limit;
        copaymentPercentage = copay;
    }

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverLimit);

        return eligible *
               (100.0 - copaymentPercentage) / 100.0;
    }

    string payerName() const override {
        return "Insurance";
    }
};

class GovernmentScheme : public Payer {
private:
    map<string, double> packageRates;

public:
    GovernmentScheme() {
        packageRates["OutPatient"] = 5000;
        packageRates["InPatient"] = 20000;
    }

    double amountCovered(const Bill& bill) const override {
        map<string, double>::const_iterator it =
            packageRates.find(bill.type());

        if (it == packageRates.end())
            throw invalid_argument("Unknown government package");

        return min(bill.total(), it->second);
    }

    string payerName() const override {
        return "Government Scheme";
    }
};

class CorporatePayer : public Payer {
private:
    double coverLimit;
    double coveragePercentage;

public:
    CorporatePayer(double limit, double percentage) {
        if (limit < 0 || percentage < 0 || percentage > 100)
            throw invalid_argument("Invalid corporate details");

        coverLimit = limit;
        coveragePercentage = percentage;
    }

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverLimit);

        return eligible * coveragePercentage / 100.0;
    }

    string payerName() const override {
        return "Corporate Tie-up";
    }
};

void displayCoverage(const Payer& payer, const Bill& bill) {
    double covered = payer.amountCovered(bill);
    double patientAmount = bill.total() - covered;

    cout << left << setw(23) << payer.payerName()
         << right << setw(12) << covered
         << setw(15) << patientAmount << '\n';
}

int main() {
    try {
        OutPatientBill bill(2000, 3000);

        SelfPay self;
        Insurance insurance(4000, 20);
        GovernmentScheme government;
        CorporatePayer corporate(4500, 80);

        cout << fixed << setprecision(2);

        cout << "HOSPITAL PAYER SYSTEM\n";
        cout << "-------------------------------\n";

        cout << "Bill Type   : " << bill.type() << '\n';
        cout << "Bill Amount : Rs. " << bill.total() << "\n\n";

        cout << left << setw(23) << "Payer"
             << right << setw(12) << "Covered"
             << setw(15) << "Patient Pays\n";

        cout << string(50, '-') << '\n';

        Payer* payers[] = {
            &self,
            &insurance,
            &government,
            &corporate
        };

        for (int i = 0; i < 4; i++) {
            displayCoverage(*payers[i], bill);
        }

        cout << "\nAll payer calculations completed.\n";
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << '\n';
    }

    return 0;
}
