//+=============================================================================
//
// file :               ClassFactory.cpp
//
// description :        C++ source for the class_factory method of the DServer
//			device class. This method is responsible to create
//			all class singletin for a device server. It is called
//			at device server startup
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//-=============================================================================

#ifndef DSERVER_IMPL_H
#define DSERVER_IMPL_H

#include <tango.h>
#include "DevTestClass.h"

namespace DevTest_ns
{

class DServerImpl : public Tango::DServer
{
        public:
        DServerImpl(Tango::DeviceClass* cl_ptr, const std::string& name, const std::string& desc, Tango::DevState state, const std::string& status):
            DServer(cl_ptr, name.c_str(), desc.c_str(), state, status.c_str())
            {}
private:
    void class_factory()
    {
        add_class(DevTestClass::init("DevTest"));
    }
};

inline Tango::DServer* constructor(Tango::DeviceClass* cl_ptr, const std::string& name, const std::string& desc, Tango::DevState state, const std::string& status)
{
        return new DServerImpl(cl_ptr, name, desc, state, status);
}
}
#endif




