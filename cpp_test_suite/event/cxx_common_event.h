#ifndef CXXCommonEvent_H
#define CXXCommonEvent_H

#include <iostream>
#include <tango.h>
#include <assert.h>

#ifdef WIN32
#include <sys/timeb.h>
#include <process.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;
using namespace Tango;

#define TEST_LOG std::cerr << "\t"

#endif // CXXCommonEvent_H
