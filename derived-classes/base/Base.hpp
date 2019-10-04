#pragma once

#include <iostream>

// Derive two classes from Base, and for each of them
// define iam() to write out the name of the class

namespace excpp
{


class Base
{
  public:
    virtual void iam() { std::cout << "Base" << std::endl; }
};


class BaseOne : public Base
{
  public:
    void iam() override { std::cout << "One" << std::endl; }
};


class BaseTwo : public Base
{
  public:
    void iam() override { std::cout << "Two" << std::endl; }
};


} // namespace excpp
