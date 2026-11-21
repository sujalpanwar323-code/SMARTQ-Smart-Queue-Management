// SMARTQ - assigned code extracted from the original project src/main.cpp
// This is a contribution file for integration, not a standalone executable.

class User
{
protected:
    string id, name;

public:
    User(string i = {}, string n = {}) : id(move(i)), name(move(n)) {}
    virtual ~User() = default;
    string getId() const { return id; }
    string getName() const { return name; }
};
class Student : public User
{
    string contact;

public:
    Student(string i = {}, string n = {}, string c = {}) : User(move(i), move(n)), contact(move(c)) {}
    string getContact() const { return contact; }
};
