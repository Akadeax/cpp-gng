#include "base.h"

// Basic C debug logger support, could use something like https://github.com/SergiusTheBest/plog
const char* BSTR(const bool b) {
    return b ? "true" : "false";
}
