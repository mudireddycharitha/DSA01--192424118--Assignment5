
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class OutPatient {
private:
    string patient;
    double consultation, tests;

public:
    OutPatient(string p, double c, double t)
        : patient(p), consultation(c), tests(t) {}

    double total() const {
        return consultation + tests;
    }

    void print() const {
        cout << "OP " << patient
             << ": " << total() << '\n';
    }
};

class InPatient {
private:
    string patient;
    int days;
    double perDay, surgery;

public:
    InPatient(string p, int d, double rate, double s)
        : patient(p), days(d), perDay(rate), surgery(s) {}

    double total() const {
        return days * perDay + surgery;
    }

    void print() const {
        cout << "IP " << patient
             << ": " << total() << '\n';
    }
};

// Template function
template <typename T>
double processBills(const vector<T>& bills) {
    double sum = 0;

    for (size_t i = 0; i < bills.size(); i++) {
        bills[i].print();
        sum += bills[i].total();
    }

    return sum;
}

int main() {
    vector<OutPatient> outpatientBills;

    vector<InPatient> inpatientBills;

    outpatientBills.push_back(
        OutPatient("Ravi", 500, 1200)
    );

    inpatientBills.push_back(
        InPatient("Meena", 3, 2500, 40000)
    );

    cout << "OUTPATIENT BILLS\n";

    double opTotal = processBills(outpatientBills);

    cout << "\nINPATIENT BILLS\n";

    double ipTotal = processBills(inpatientBills);

    cout << "\nTotal Hospital Revenue: "
         << opTotal + ipTotal << '\n';

    return 0;
}
