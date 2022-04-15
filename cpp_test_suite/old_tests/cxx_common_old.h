#ifndef CXXCommonOld_H
#define CXXCommonOld_H

#include <iostream>
#include <tango.h>
#include <assert.h>

// TODO 2022-06-01, tjuerges
// Temporary include to satisfy compilation units where 'Tango::EventData'
// cannot be resolved.
// After the header file separation this will be unnecessary.
#ifdef __darwin__
#include "../../../cppapi/client/event.h"
#endif

using namespace std;
using namespace Tango;

#define TEST_LOG std::cerr << "\t"

#endif // CXXCommonOld_H
