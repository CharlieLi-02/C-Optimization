#include "Nodes.h"

// Implement your AST subclasses' member functions here.

virtual std::string prefix()  const = 0;
virtual std::string postfix() const = 0;
virtual double value()   const = 0;

// To format a double for output:
//   std::ostringstream stream;
//   stream << value;
//   return stream.str();
