#include <iostream>
#include <string>
#include <iomanip>

struct Date {
    int day, month, year;
    std::string str() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
};

class Vehicle {
protected:
    std::string regNo;
    double dailyRate;
    bool rented = false;

public:
    Vehicle(std::string reg, double rate) : regNo(std::move(reg)), dailyRate(rate) {}
    virtual ~Vehicle() = default;

    std::string getRegNo() const { return regNo; }
    double getDailyRate() const { return dailyRate; }
    bool isRented() const { return rented; }
    void setRented(bool status) { rented = status; }

    virtual double calculateCharge(int days, double energyUsed) const = 0;
    virtual void display() const = 0;
};

class Bicycle : public Vehicle {
public:
    Bicycle(std::string reg, double rate) : Vehicle(std::move(reg), rate) {}

    double calculateCharge(int days, double /*energyUsed*/) const override {
        return days * dailyRate;
    }

    void display() const override {
        std::cout << "[Bicycle] " << regNo << " | Rate: " << dailyRate
                  << "/day | Status: " << (rented ? "Rented" : "Available") << '\n';
    }
};

class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel;
    Date serviceDue;
    static constexpr double PETROL_RATE = 102.0;

public:
    MotorVehicle(std::string reg, double rate, double level, Date due)
        : Vehicle(std::move(reg), rate), fuelOrChargeLevel(level), serviceDue(due) {}
};

class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(std::string reg, double rate, double fuel, Date due)
        : MotorVehicle(std::move(reg), rate, fuel, due) {}

    double calculateCharge(int days, double litresUsed) const override {
        return (days * dailyRate) + (litresUsed * PETROL_RATE);
    }

    void display() const override {
        std::cout << "[TwoWheeler] " << regNo << " | Rate: " << dailyRate
                  << "/day | Fuel: " << fuelOrChargeLevel << "% | Service: "
                  << serviceDue.str() << " | Status: " << (rented ? "Rented" : "Available") << '\n';
    }
};

class Car : public MotorVehicle {
protected:
    int seatingCapacity;

public:
    Car(std::string reg, double rate, double fuel, Date due, int seats)
        : MotorVehicle(std::move(reg), rate, fuel, due), seatingCapacity(seats) {}

    double calculateCharge(int days, double litresUsed) const override {
        return (days * dailyRate) + (litresUsed * PETROL_RATE);
    }

    void display() const override {
        std::cout << "[Car] " << regNo << " | Seats: " << seatingCapacity
                  << " | Rate: " << dailyRate << "/day | Service: " << serviceDue.str()
                  << " | Status: " << (rented ? "Rented" : "Available") << '\n';
    }
};

class ElectricCar : public Car {
protected:
    double batteryCapacityKWh;
    double ratePerKWh;

public:
    ElectricCar(std::string reg, double rate, double chargePct, Date due,
                int seats, double capKWh, double kwhRate)
        : Car(std::move(reg), rate, chargePct, due, seats),
          batteryCapacityKWh(capKWh), ratePerKWh(kwhRate) {}

    double calculateCharge(int days, double kWhUsed) const override {
        return (days * dailyRate) + (kWhUsed * ratePerKWh);
    }

    void display() const override {
        std::cout << "[ElectricCar] " << regNo << " | Seats: " << seatingCapacity
                  << " | Battery: " << batteryCapacityKWh << "kWh | Rate: " << dailyRate
                  << "/day | Status: " << (rented ? "Rented" : "Available") << '\n';
    }
};

class ElectricBus : public ElectricCar {
    double commercialPermitFee;
public:
    ElectricBus(std::string reg, double rate, double chargePct, Date due,
                int seats, double capKWh, double kwhRate, double permitFee)
        : ElectricCar(std::move(reg), rate, chargePct, due, seats, capKWh, kwhRate),
          commercialPermitFee(permitFee) {}

    double calculateCharge(int days, double kWhUsed) const override {
        return ElectricCar::calculateCharge(days, kWhUsed) + commercialPermitFee;
    }

    void display() const override {
        std::cout << "[ElectricBus] " << regNo << " | Seats: " << seatingCapacity
                  << " | Battery: " << batteryCapacityKWh << "kWh | Permit: "
                  << commercialPermitFee << " | Status: " << (rented ? "Rented" : "Available") << '\n';
    }
};

void rentVehicle(Vehicle* fleet[], int count, const std::string& reg) {
    for (int i = 0; i < count; ++i) {
        if (fleet[i]->getRegNo() == reg && !fleet[i]->isRented()) {
            fleet[i]->setRented(true);
            std::cout << "Rented: " << reg << '\n';
            return;
        }
    }
    std::cout << "Cannot rent: " << reg << '\n';
}

void returnVehicle(Vehicle* fleet[], int count, const std::string& reg,
                   int days, int lateDays, double energyUsed) {
    for (int i = 0; i < count; ++i) {
        if (fleet[i]->getRegNo() == reg && fleet[i]->isRented()) {
            double base = fleet[i]->calculateCharge(days, energyUsed);
            double penalty = lateDays * (fleet[i]->getDailyRate() * 1.5);
            fleet[i]->setRented(false);
            std::cout << "Returned " << reg << " | Base+Energy: " << base
                      << " | Late Fee: " << penalty << " | Total: " << (base + penalty) << '\n';
            return;
        }
    }
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    Vehicle* fleet[5] = {
        new Bicycle("CYC-01", 150.0),
        new TwoWheeler("TN01-AB-1234", 500.0, 85.0, {15, 12, 2026}),
        new Car("TN02-CD-5678", 2000.0, 90.0, {20, 1, 2027}, 5),
        new ElectricCar("TN03-EV-9999", 2800.0, 95.0, {10, 3, 2027}, 5, 60.0, 18.0),
        new ElectricBus("TN04-EB-7777", 8500.0, 100.0, {1, 5, 2027}, 40, 250.0, 16.0, 1200.0)
    };

    for (Vehicle* v : fleet) rentVehicle(fleet, 5, v->getRegNo());
    std::cout << "\n--- Active Fleet ---\n";
    for (Vehicle* v : fleet) v->display();

    std::cout << "\n--- Processing Returns (with 1 late day on ElectricCar & ElectricBus) ---\n";
    returnVehicle(fleet, 5, "CYC-01", 3, 0, 0);
    returnVehicle(fleet, 5, "TN01-AB-1234", 2, 0, 4.5);
    returnVehicle(fleet, 5, "TN02-CD-5678", 4, 0, 18.0);
    returnVehicle(fleet, 5, "TN03-EV-9999", 3, 1, 45.0);
    returnVehicle(fleet, 5, "TN04-EB-7777", 2, 1, 180.0);

    for (Vehicle* v : fleet) delete v;
    return 0;
}
