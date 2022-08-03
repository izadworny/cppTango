#include <thread>
#include <chrono>

#include "common.h"

class EventCallBack : public Tango::CallBack
{
	void push_event(Tango::EventData*);

public:
	int cb_executed;
	int cb_err;
	int old_sec,old_usec;
	DevState sta;
	std::string status;
};

void EventCallBack::push_event(Tango::EventData* event_data)
{
	struct timeval now_timeval = Tango::make_timeval(std::chrono::system_clock::now());

	TEST_LOG << "date : tv_sec = " << now_timeval.tv_sec;
	TEST_LOG << ", tv_usec = " << now_timeval.tv_usec << std::endl;

	auto delta_msec = ((now_timeval.tv_sec - old_sec) * 1000) + ((now_timeval.tv_usec - old_usec) / 1000);

	old_sec = now_timeval.tv_sec;
	old_usec = now_timeval.tv_usec;

	TEST_LOG << "delta_msec = " << delta_msec << std::endl;

	cb_executed++;

	try
	{
		TEST_LOG << "StateEventCallBack::push_event(): called attribute " << event_data->attr_name << " event " << event_data->event << "\n";
		if (!event_data->err)
		{
			std::string::size_type pos = event_data->attr_name.find_last_of('/');
			pos++;
			std::string att_name = event_data->attr_name.substr(pos);

			if (att_name == "state")
			{
				*(event_data->attr_value) >> sta;
				TEST_LOG << "State in callback: " << DevStateName[sta] << std::endl;
			}
			else if (att_name == "status")
			{
				*(event_data->attr_value) >> status;
				TEST_LOG << "Status in callback: " << status << std::endl;
			}
			else
				TEST_LOG << "Received callback for an unknown attribute : " << event_data->attr_name << std::endl;
		}
		else
		{
			TEST_LOG << "Error send to callback" << std::endl;
		}
	}
	catch (...)
	{
		cb_err++;
		TEST_LOG << "EventCallBack::push_event(): could not extract data !\n";
	}

}

int main(int argc, char **argv)
{
	DeviceProxy *device;

	if (argc == 1)
	{
		TEST_LOG << "usage: %s device" << std::endl;
		exit(-1);
	}

	std::string device_name = argv[1];

	try
	{
		device = new DeviceProxy(device_name);
	}
	catch (CORBA::Exception &e)
	{
		Except::print_exception(e);
		exit(1);
	}

	TEST_LOG << std::endl << "new DeviceProxy(" << device->name() << ") returned" << std::endl << std::endl;

	try
	{
		std::string att_name("State");

//
// Test set up (stop polling)
//

		if (device->is_attribute_polled(att_name))
			device->stop_poll_attribute(att_name);

		DeviceProxy adm_dev(device->adm_name().c_str());
		DeviceData di;
		di << device_name;
		adm_dev.command_inout("DevRestart",di);

		delete device;
		device = new DeviceProxy(device_name);
		std::this_thread::sleep_for(std::chrono::seconds(1));

//
// subscribe to a state change event
//

		int eve_id;
		std::vector<std::string> filters;
		EventCallBack cb;
		cb.cb_executed = 0;
		cb.cb_err = 0;
		cb.old_sec = cb.old_usec = 0;

		// start the polling first!
		device->poll_attribute(att_name,1000);
		eve_id = device->subscribe_event(att_name,Tango::CHANGE_EVENT,&cb,filters);

//
// Check that the attribute is now polled at 1000 mS
//

		bool po = device->is_attribute_polled(att_name);
		TEST_LOG << "attribute polled : " << po << std::endl;
		assert( po == true);

		po = device->is_command_polled(att_name);
		assert( po == true);

		int poll_period = device->get_attribute_poll_period(att_name);
		TEST_LOG << "att polling period : " << poll_period << std::endl;
		assert( poll_period == 1000);

		TEST_LOG << "   subscribe_event on state attribute --> OK" << std::endl;

//
// Check that first point has been received
//

		assert (cb.cb_executed == 1);
		assert (cb.sta == Tango::ON);
		assert (cb.cb_err == 0);
		TEST_LOG << "   first point received --> OK" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));

//
// Change the device state and check that an event has been received
//

		DeviceData dd;
		DevState d_state = Tango::OFF;
		dd << d_state;
		device->command_inout("IOState",dd);

		std::this_thread::sleep_for(std::chrono::seconds(1));

		d_state = Tango::ON;
		dd << d_state;
		device->command_inout("IOState",dd);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		assert (cb.cb_executed == 4);
		assert (cb.cb_err == 0);
		assert (cb.sta == Tango::ON);

		TEST_LOG << "   Event on state change --> OK" << std::endl;

//
// Execute command manually push a change event on state
//

		device->command_inout("PushStateStatusChangeEvent");
		std::this_thread::sleep_for(std::chrono::seconds(1));

		assert (cb.cb_executed >= 4);
		assert (cb.cb_err == 0);
		assert (cb.sta == Tango::ON);

		TEST_LOG << "   Event on state manually pushed --> OK" << std::endl;

//
// unsubscribe to the event
//

		device->unsubscribe_event(eve_id);
//
// Stop polling
//
		device->stop_poll_attribute(att_name);

		TEST_LOG << "   unsubscribe_event --> OK" << std::endl;

//
// Clear data in callback object and subscribe to status event
//

		cb.cb_executed = 0;
		att_name = "status";

		// start the polling first!
		device->poll_attribute(att_name,1000);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		eve_id = device->subscribe_event(att_name,Tango::CHANGE_EVENT,&cb,filters);
		TEST_LOG << "   subscribe_event on status attribute --> OK" << std::endl;

//
// Check that first point has been received
//

		assert (cb.cb_executed == 1);
		assert (cb.cb_err == 0);
		TEST_LOG << "   first point received --> OK" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));

//
// Change the device state and check that an event has been received
// Changing the device state obviously change the device status
//

		d_state = Tango::OFF;
		dd << d_state;
		device->command_inout("IOState",dd);

		std::this_thread::sleep_for(std::chrono::seconds(1));

		d_state = Tango::ON;
		dd << d_state;
		device->command_inout("IOState",dd);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		std::string::size_type pos = cb.status.find("ON");
		assert (cb.cb_executed == 4);
		assert (pos != std::string::npos);
		assert (cb.cb_err == 0);

		TEST_LOG << "   Event on status change --> OK" << std::endl;

//
// Execute command manually push a change event on status
//

		device->command_inout("PushStateStatusChangeEvent");
		std::this_thread::sleep_for(std::chrono::seconds(1));

		assert (cb.cb_executed >= 4);
		assert (cb.cb_err == 0);

		TEST_LOG << "   Event on status manually pushed --> OK" << std::endl;

//
// unsubscribe to the event
//

		device->unsubscribe_event(eve_id);

		TEST_LOG << "   unsubscribe_event --> OK" << std::endl;

//
// Stop polling
//

		device->stop_poll_attribute(att_name);
	}
	catch (Tango::DevFailed &e)
	{
		Except::print_exception(e);
		exit(-1);
	}
	catch (CORBA::Exception &ex)
	{
		Except::print_exception(ex);
		exit(-1);
	}

	delete device;

Tango::ApiUtil::cleanup();

	return 0;
}
