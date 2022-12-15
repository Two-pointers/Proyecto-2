#pragma once
#include <string>
#include "../coloring/coloring.hpp"
using namespace std;

struct Error{
  string description;

  Error()
    : description ("")
    {};
  
  Error (string description)
    : description (description)
    {};
  
  string toString();
};