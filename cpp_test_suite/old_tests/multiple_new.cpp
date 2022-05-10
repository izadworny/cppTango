#include "cxx_common_old.h"

int main(int argc, char **argv)
{
	DeviceProxy *device1,*device2,*device3;
	
	if (argc != 5)
	{
		TEST_LOG << "usage: %s device1 device2 device3 loop" << endl;
		exit(-1);
	}

	string device_name1 = argv[1];
	string device_name2 = argv[2];
	string device_name3 = argv[3];
	long loop = atoi(argv[4]);

//
// First device
//

	for (int k = 0;k < loop;k++)
	{
		try 
		{
			device1 = new DeviceProxy(device_name1);
			TEST_LOG << endl << "new DeviceProxy(" << device1->name() << ") returned" << endl << endl;
			
			device1->ping();
			TEST_LOG << "Ping on device " << device1->name() << " successfull" << endl;
		}
       	 	catch (CORBA::Exception &e)
       	 	{
              		Except::print_exception(e);
			exit(1);
        	}

//
// Second device
//
	
		try 
		{
			device2 = new DeviceProxy(device_name2);
			TEST_LOG << endl << "new DeviceProxy(" << device2->name() << ") returned" << endl << endl;
			
			device2->ping();
			TEST_LOG << "Ping on device " << device2->name() << " successfull" << endl;
		}
        	catch (CORBA::Exception &e)
        	{
              		Except::print_exception(e);
			exit(1);
        	}

//
// Third device
//
	
		try 
		{
			device3 = new DeviceProxy(device_name3);
			TEST_LOG << endl << "new DeviceProxy(" << device3->name() << ") returned" << endl << endl;
			
			device3->ping();
			TEST_LOG << "Ping on device " << device3->name() << " successfull" << endl;
		}
		catch (CORBA::Exception &e)
		{
			Except::print_exception(e);
			exit(1);
		}

//
// print db object 
//

		vector<Database *> dbs = ApiUtil::instance()->get_db_vect();
		TEST_LOG << "Db objects number = " << dbs.size() << endl;
		for (unsigned int i = 0;i < dbs.size();i++)
		{
			TEST_LOG << "database host = " << dbs[i]->get_db_host() << endl;
			TEST_LOG << "database port = " << dbs[i]->get_db_port() << endl;
		}
	
		delete device1;
		delete device2;
		delete device3;
	}

	return 0;
}
