#ifndef ATLAS_H
#define ATLAS_H

#include <istream>
#include <string>

#include "Trip.h"
#include "Helpers.h"

class Atlas {
public:
  // Required Class Function
  static Atlas* create(std::istream& stream);
  Station* station = NULL;
  AMGGraph* AMG = NULL;
  Trip* trip = NULL;
  AGraph* G = NULL;
private:
  // Member Variables

public:
  // Constructor & Destructor
  Atlas(std::istream& stream);
  ~Atlas();

  // Required Member Function
  Trip route(const std::string& src, const std::string& dst);
};

#endif
