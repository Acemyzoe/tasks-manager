#include "Singleton.h"
#include <iostream>
class DerivedSingle : public Singleton<DerivedSingle>
{
public:
    DerivedSingle(token)
    {
        std::cout << "constructor called!" << std::endl;
    }

    ~DerivedSingle()
    {
        std::cout << "destructor called!" << std::endl;
    }
    // DerivedSingle(const DerivedSingle &) = delete;            // disable copy constructor
    // DerivedSingle &operator=(const DerivedSingle &) = delete; // disable copy assignment
};

int main(int argc, char *argv[])
{
    DerivedSingle &instance1 = DerivedSingle::instance();
    DerivedSingle &instance2 = DerivedSingle::instance();
    return 0;
}