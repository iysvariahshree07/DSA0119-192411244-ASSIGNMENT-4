#include <iostream>
#include <string>

class Address {
    std::string city;
    int pin;
public:
    Address(std::string c, int p) : city(std::move(c)), pin(p) {
        std::cout << "[Ctor] Address\n";
    }
    ~Address() { std::cout << "[Dtor] Address\n"; }
};

class Course {
    std::string code;
public:
    Course(std::string c) : code(std::move(c)) {
        std::cout << "[Ctor] Course\n";
    }
    ~Course() { std::cout << "[Dtor] Course\n"; }
};

class Person {
protected:
    std::string name;
    Address addr;
public:
    Person(std::string n, std::string city, int pin)
        : name(std::move(n)), addr(std::move(city), pin) {
        std::cout << "[Ctor] Person\n";
    }
    virtual ~Person() { std::cout << "[Dtor] Person\n"; }
};

class Student : virtual public Person {
protected:
    double cgpa;
    Course major;
public:
    Student(std::string n, std::string city, int pin, double c, std::string courseCode)
        : Person(std::move(n), std::move(city), pin), cgpa(c), major(std::move(courseCode)) {
        std::cout << "[Ctor] Student\n";
    }
    ~Student() override { std::cout << "[Dtor] Student\n"; }
};

class Staff : virtual public Person {
protected:
    double salary;
public:
    Staff(std::string n, std::string city, int pin, double sal)
        : Person(std::move(n), std::move(city), pin), salary(sal) {
        std::cout << "[Ctor] Staff\n";
    }
    ~Staff() override { std::cout << "[Dtor] Staff\n"; }
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(std::string n, std::string city, int pin,
                      double c, std::string courseCode, double sal)
        : Person(n, city, pin),
          Student(n, city, pin, c, std::move(courseCode)),
          Staff(n, city, pin, sal) {
        std::cout << "[Ctor] TeachingAssistant\n";
    }
    ~TeachingAssistant() override { std::cout << "[Dtor] TeachingAssistant\n"; }
};

int main() {
    std::cout << "=== Student ===\n";
    { Student s("Anu", "Chennai", 600001, 9.1, "DSA01"); }

    std::cout << "\n=== Staff ===\n";
    { Staff st("Ravi", "Madurai", 625001, 55000); }

    std::cout << "\n=== TeachingAssistant (virtual) ===\n";
    { TeachingAssistant ta("Kiran", "Salem", 636001, 9.4, "DSA01", 25000); }

    std::cout << "\nsizeof(TeachingAssistant) WITH virtual: " << sizeof(TeachingAssistant) << " bytes\n";
    return 0;
}
