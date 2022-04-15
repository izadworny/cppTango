#ifndef CXXCommon_H
#define CXXCommon_H

#include <cxxtest/TestSuite.h>
#include <cxxtest/TangoPrinter.h>
#include <iostream>
#ifdef __darwin__
// TODO 2022-06-17, tjuerges
// Temporary include to satisfy compilation units where 'Tango::EventData'
// and company cannot be resolved.
// After the header file separation
// (https://gitlab.com/tango-controls/cppTango/-/issues/735) this will be
// unnecessary.
#include "../../../cppapi/client/event.h"
#endif
#include <tango.h>

using namespace std;
using namespace Tango;

#include "cxx_logging.h"

#endif // CXXCommon_H
