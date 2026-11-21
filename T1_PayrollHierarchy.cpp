#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

class Staff {
protected:
    std::string name;
    int id;
    static constexpr double DA_RATE = 0.42;

public:
    Staff(std::string n, int staffId) : name(std::move(n)), id(staffId) {}
    virtual ~Staff() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    virtual double pay() const = 0;
};

class Professor : public Staff {
private:
    double basic;
    double research;

public:
    Professor(std::string n, int id, double b, double r)
        : Staff(std::move(n), id), basic(b), research(r) {}

    double pay() const override {
        return basic + (basic * DA_RATE) + research;
    }
};

class LabAssistant : public Staff {
private:
    double basic;

public:
    LabAssistant(std::string n, int id, double b)
        : Staff(std::move(n), id), basic(b) {}

    double pay() const override {
        return basic + (basic * DA_RATE);
    }
};

class GuestFaculty : public Staff {
private:
    double hours;
    double rate;

public:
    GuestFaculty(std::string n, int id, double h, double r)
        : Staff(std::move(n), id), hours(h), rate(r) {}

    double pay() const override {
        return hours * rate;
    }
};

int main() {
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Dr. Rao", 101, 80000, 15000));
    payroll.push_back(std::make_unique<LabAssistant>("Karthik", 102, 35000));
    payroll.push_back(std::make_unique<GuestFaculty>("Meera", 103, 40, 1200));

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& s : payroll) {
        std::cout << "ID: " << s->getId() << " | Name: " << s->getName()
                  << " | Pay: " << s->pay() << '\n';
    }
    return 0;
}
