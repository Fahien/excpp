#ifndef CPP_IOEX_H_
#define CPP_IOEX_H_


#include <iostream>
#include <stdexcept>


namespace excpp
{

class IOException : public std::runtime_error
{
  public:
	IOException(const std::string& msg) : std::runtime_error{ msg } {}
};

FILE*  fopenEx(const std::string& name, const std::string& mode);

FILE*  fopenEx(const char* name, const char* mode);

size_t freadEx(void* buffer, size_t size, size_t count, FILE* stream);

size_t fwriteEx(const void* buffer, size_t size, size_t count, FILE* stream);

int    fcloseEx(FILE* stream);

};


#endif // CPP_IOEX_H_
