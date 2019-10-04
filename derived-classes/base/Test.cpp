#include <cstdlib>
#include <iostream>

#include "Base.hpp"

// Create objects of the Base hierarchy and call iam() for them.
// Assign pointers to objects of the derived classes to Base* pointers
// and call iam() through those pointers.

int main()
{
    using namespace excpp;

    Base b;
    b.iam(); // Base

    BaseOne one;
    one.iam(); // One

    BaseTwo two;
    two.iam(); // Two

    Base* pto_one = &one;
    pto_one->iam(); // One

    Base* pto_two = &two;
    pto_two->iam(); // Two

    return EXIT_SUCCESS;
}
