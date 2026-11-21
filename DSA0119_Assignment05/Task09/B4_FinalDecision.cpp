
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

using namespace std;

class Bill {
public:
    virtual double total() const = 0;
    virtual string type() const = 0;

    virtual ~Bill() {
    }
};

class OutPatientBill : public Bill {
private:
    double consultationFee;
    double testCharges;

public:
    OutPatientBill(double consultation, double tests)
        : consultationFee(consultation),
          testCharges(tests) {
    }

    double total() const override {
        return consultationFee + testCharges;
    }

    string type() const override {
        return "OutPatient";
    }
};

class InPatientBill : public Bill {
private:
    int days;
    double dailyRate;
    double additionalCharges;

public:
    InPatientBill(int d, double rate, double charges)
        : days(d),
          dailyRate(rate),
          additionalCharges(charges) {
    }

    double total() const override {
        return days * dailyRate + additionalCharges;
    }

    string type() const override {
        return "InPatient";
    }
};

class DayCareBill : public Bill {
private:
    double procedureCharges;

public:
    DayCareBill(double charges)
        : procedureCharges(charges) {
    }

    double total() const override {
        return procedureCharges;
    }

    string type() const override {
        return "DayCare";
    }
};

class EmergencyBill : public Bill {
private:
    double treatmentCharges;
    int emergencyHours;
    double hourlyRate;

public:
    EmergencyBill(double treatment, int hours, double rate)
        : treatmentCharges(treatment),
          emergencyHours(hours),
          hourlyRate(rate) {
    }

    double total() const override {
        return treatmentCharges + emergencyHours * hourlyRate;
    }

    string type() const override {
        return "Emergency";
    }
};

class Payer {
public:
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual string name() const = 0;

    virtual ~Payer() {
    }
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override {
        return 0;
    }

    string name() const override {
        return "SelfPay";
    }
};

class Insurance : public Payer {
private:
    double coverageLimit;
    double coPayment;

public:
    Insurance(double limit, double copay)
        : coverageLimit(limit),
          coPayment(copay) {
    }

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverageLimit);

        return eligible * (100.0 - coPayment) / 100.0;
    }

    string name() const override {
        return "Insurance";
    }
};

class GovernmentScheme : public Payer {
public:
    double amountCovered(const Bill& bill) const override {
        double packageLimit = 0;

        if (bill.type() == "OutPatient")
            packageLimit = 5000;
        else if (bill.type() == "InPatient")
            packageLimit = 20000;
        else if (bill.type() == "DayCare")
            packageLimit = 12000;
        else if (bill.type() == "Emergency")
            packageLimit = 15000;

        return min(bill.total(), packageLimit);
    }

    string name() const override {
        return "Government";
    }
};

class CorporatePayer : public Payer {
private:
    double coverageLimit;

public:
    CorporatePayer(double limit)
        : coverageLimit(limit) {
    }

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverageLimit);

        return eligible * 0.80;
    }

    string name() const override {
        return "Corporate";
    }
};

class MaternityBill : public Bill {
private:
    double consultationFee;
    double deliveryFee;
    int roomDays;
    double dailyRoomRate;

public:
    MaternityBill(double consultation,
                  double delivery,
                  int days,
                  double roomRate)
        : consultationFee(consultation),
          deliveryFee(delivery),
          roomDays(days),
          dailyRoomRate(roomRate) {

        if (consultation < 0 || delivery < 0 ||
            days < 0 || roomRate < 0) {
            throw invalid_argument(
                "Maternity bill values cannot be negative"
            );
        }
    }

    double total() const override {
        return consultationFee + deliveryFee
               + roomDays * dailyRoomRate;
    }

    string type() const override {
        return "Maternity";
    }
};

class CharityPayer : public Payer {
private:
    double coveragePercent;
    double maximumCoverage;

public:
    CharityPayer(double percentage, double maximum)
        : coveragePercent(percentage),
          maximumCoverage(maximum) {

        if (percentage < 0 || percentage > 100 ||
            maximum < 0) {
            throw invalid_argument(
                "Invalid charity coverage settings"
            );
        }
    }

    double amountCovered(const Bill& bill) const override {
        double coverage = bill.total() *
                          coveragePercent / 100.0;

        return min(coverage, maximumCoverage);
    }

    string name() const override {
        return "Charity";
    }
};

class TestResource {
private:
    string resourceName;

public:
    TestResource(const string& name)
        : resourceName(name) {
        cout << resourceName << " created\n";
    }

    ~TestResource() {
        cout << resourceName << " destroyed\n";
    }
};

void testExceptionHandling() {
    cout << "\n===== EXCEPTION AND RESOURCE TEST =====\n";

    try {
        TestResource resource("BillingResource");

        throw runtime_error("Simulated billing failure");
    }
    catch (const exception& e) {
        cout << "Caught exception: " << e.what() << '\n';
    }

    cout << "Exception test completed\n";
}

void displayResults(
    const vector<unique_ptr<Bill>>& bills,
    const vector<unique_ptr<Payer>>& payers
) {
    cout << "\n===== BILLING REGRESSION TESTS =====\n";

    cout << fixed << setprecision(2);

    for (const auto& bill : bills) {
        for (const auto& payer : payers) {
            double billTotal = bill->total();

            double covered =
                payer->amountCovered(*bill);

            double patientContribution =
                billTotal - covered;

            cout << "\nBill: " << bill->type()
                 << " | Payer: " << payer->name() << '\n';

            cout << "Bill Total: Rs. "
                 << billTotal << '\n';

            cout << "Covered Amount: Rs. "
                 << covered << '\n';

            cout << "Patient Contribution: Rs. "
                 << patientContribution << '\n';
        }
    }
}

int main() {
    cout << fixed << setprecision(2);

    vector<unique_ptr<Bill>> bills;

    bills.push_back(
        make_unique<OutPatientBill>(500, 1200)
    );

    bills.push_back(
        make_unique<InPatientBill>(3, 2500, 40000)
    );

    bills.push_back(
        make_unique<DayCareBill>(9000)
    );

    bills.push_back(
        make_unique<EmergencyBill>(3000, 4, 2000)
    );


    bills.push_back(
        make_unique<MaternityBill>(2000, 30000, 3, 4000)
    );

    vector<unique_ptr<Payer>> payers;

    payers.push_back(
        make_unique<SelfPay>()
    );

    payers.push_back(
        make_unique<Insurance>(30000, 20)
    );

    payers.push_back(
        make_unique<GovernmentScheme>()
    );

    payers.push_back(
        make_unique<CorporatePayer>(25000)
    );

    payers.push_back(
        make_unique<CharityPayer>(50, 25000)
    );

    displayResults(bills, payers);

    cout << "\n===== NEW EXTENSION VERIFICATION =====\n";

    MaternityBill maternity(2000, 30000, 3, 4000);

    CharityPayer charity(50, 25000);

    double total = maternity.total();

    double covered = charity.amountCovered(maternity);

    cout << "New Bill Type: " << maternity.type() << '\n';

    cout << "New Payer Type: " << charity.name() << '\n';

    cout << "Maternity Bill Total: Rs. "
         << total << '\n';

    cout << "Charity Coverage: Rs. "
         << covered << '\n';

    cout << "Patient Contribution: Rs. "
         << total - covered << '\n';


    testExceptionHandling();

    cout << "\n===== ALL B4 TESTS COMPLETED =====\n";

    return 0;
}
