#include "cxx_common_old.h"

int main(int argc, char **argv)
{
	DeviceProxy *device;
	
	if (argc != 2)
	{
		TEST_LOG << "usage: %s device" << endl;
		exit(-1);
	}

	string device_name = argv[1];

	try 
	{
		device = new DeviceProxy(device_name);
	}
        catch (CORBA::Exception &e)
        {
              	Except::print_exception(e);
		exit(1);
        }

	TEST_LOG << endl << "new DeviceProxy(" << device->name() << ") returned" << endl << endl;


for (int i = 0;i < 5;i++)
{
	DeviceAttribute da;
	try
	{	
		da = device->read_attribute("Long_attr");
	}
	catch (CORBA::Exception &e)
	{
		Except::print_exception(e);
		exit(-1);
	}
	TEST_LOG << "read_attribute successfull" << endl;
}	

	delete device;

//	sleep(1000);

		

}
