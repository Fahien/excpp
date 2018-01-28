/// A function taking arguments of type pointer to char
/// and reference to integer and returning no value;
void func(char* key, int& value);

/// A pointer to func
using FuncType = void (*)(char*, int&);

/// A function taking such a pointer as an argument
void SetFunc(FuncType f);

/// A dunction returning such a pointer
FuncType GetFunc();

/// A function that takes such a pointer as an argument
/// and returns its argument as the return value
FuncType Identity(FuncType);
