#pragma once 
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <variant> 
#include "../utils/types/error.hpp"
using namespace std;

variant<Error,string> myers(string v1, string v2);
