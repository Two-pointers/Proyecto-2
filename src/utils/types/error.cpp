#include "error.hpp"


string Error::toString(){
  string errString = "[ERROR] " + this->description;
  return colorRED(errString);
}