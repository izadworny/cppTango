//+=============================================================================
//
// file :               main.cpp
//
// description :        C++ source for a TANGO device server main.
//			The main rule is to initialise (and create) the Tango
//			system and to create the DServerClass singleton.
//			The main should be the same for every Tango device server.
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

#include <tango.h>
#include <dserverclass.h>
#include <dserversignal.h>


int main(int argc,char *argv[])
{
	Tango::Util *tg = nullptr;
	try
	{

		tg = Tango::Util::init(argc,argv);

		tg->server_init();

		tg->server_run();
	}
	catch (std::bad_alloc&)
	{
		std::cout << "Can't allocate memory to store device object !!!" << std::endl;
		std::cout << "Exiting" << std::endl;
	}
	catch (Tango::DevFailed &e)
	{
		Tango::Except::print_exception(e);
	}
	catch (CORBA::Exception &e)
	{
		Tango::Except::print_exception(e);

		std::cout << "Received a CORBA_Exception" << std::endl;
		std::cout << "Exiting" << std::endl;
	}

//
// Destroy the ORB (and properly releases its resources)
//
	Tango::Util::instance()->server_cleanup();
	return(0);
}
