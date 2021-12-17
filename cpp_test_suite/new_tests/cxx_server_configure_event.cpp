//
// Created by pxh on 18.12.2021.
//
#ifndef ServerConfigureEventTestSuite_h
#define ServerConfigureEventTestSuite_h


#include <cxxtest/TestSuite.h>
#include <cxxtest/TangoPrinter.h>
#include <tango.h>
#include <iostream>

using namespace Tango;
using namespace std;

#define cout cout << "\t"
#define    coutv    if (verbose == true) cout

#undef SUITE_NAME
#define SUITE_NAME ServerConfigureEventTestSuite

template <typename TEvent>
struct EventCallback : public Tango::CallBack
{
    EventCallback()
        : num_of_all_events(0)
        , num_of_error_events(0)
    {}

    void push_event(TEvent* event)
    {
        num_of_all_events++;
        if (event->err)
        {
            num_of_error_events++;
        }
    }

    int num_of_all_events;
    int num_of_error_events;
};

class ServerConfigureEventTestSuite : public CxxTest::TestSuite {
protected:
    string dserver_name, device1_name, device1_instance_name;
    string event_port, event_port_str, heartbeat_port, heartbeat_port_str;
    string::size_type pos;
    bool verbose;

public:
    SUITE_NAME():
            device1_instance_name{"test"}
            {

//
// Arguments check -------------------------------------------------
//

        event_port = "9977";
        heartbeat_port = "9988";
    		device1_name = CxxTest::TangoPrinter::get_param("device1");
    		dserver_name = "dserver/" + CxxTest::TangoPrinter::get_param("fulldsname");
        verbose = CxxTest::TangoPrinter::is_param_defined("verbose");
        CxxTest::TangoPrinter::validate_args();
    }

    virtual ~SUITE_NAME() {
        // Cleanup servers if necessary
        CxxTest::TangoPrinter::kill_server();

        // make sure that device1 is actually running as it expected
        // by subsequent tests
        CxxTest::TangoPrinter::start_server(device1_instance_name);
    }

    static SUITE_NAME *createSuite() {
        return new SUITE_NAME();
    }

    static void destroySuite(SUITE_NAME *suite) {
        delete suite;
    }

//
// Tests -------------------------------------------------------
//

//
// Ask the device server to subscribe to an event and then check the
// ZmqEventSubscriptionChange ["info"] results to see whether it has
// the fixed event and heartbeat ports on the DS
//
    void test_device_server_zmq_event_port_configured(void) {

        DeviceProxy *dserver;
        DeviceProxy *device1;

        try {
            // Stop any previously running server first
            CxxTest::TangoPrinter::kill_server();

            // Restart the test server with env vars for the fixed ports
            std::string env = "TANGO_ZMQ_EVENT_PORT="+event_port+" TANGO_ZMQ_HEARTBEAT_PORT="+heartbeat_port+" ";
            coutv << endl << "test_device_server_zmq_event_port_configured: relaunching DevTest/test with fixed zmq ports - " << env + Tango::kStartServerCmd + device1_instance_name << endl;
            system((env + Tango::kStartServerCmd + device1_instance_name).c_str());

            CxxTest::TangoPrinter::restore_set("test/debian8/10 started.");
      			device1 = new DeviceProxy(device1_name);
      			dserver = new DeviceProxy(dserver_name);
      			device1->ping();
      			dserver->ping();
        }
        catch (CORBA::Exception &e) {
            Except::print_exception(e);
            exit(-1);
        }

        coutv << endl << "test_device_server_zmq_event_port_configured: new DeviceProxy(" << device1->name() +"[" + device1_name +"]" << ") returned" << endl;

        // setup a subscription so that the event port is woken up
        EventCallback<Tango::EventData> callback{};
        std::string attribute_name = "event_change_tst";
        TS_ASSERT_THROWS_NOTHING(device1->subscribe_event(
            attribute_name,
            Tango::USER_EVENT,
            &callback));
        coutv << endl << "test_device_server_zmq_event_port_configured: sending IOPushEvent" << endl;
        TS_ASSERT_THROWS_NOTHING(device1->command_inout("IOPushEvent"));
        Tango_sleep(2);

        // query the admin device so that we get the zmq port specifications
        coutv << endl << "test_device_server_zmq_event_port_configured: sending ZmqEventSubscriptionChange" << endl << endl;
    		DeviceData din,dout;
				vector<string> din_info;
				din_info.push_back("info");
				din << din_info;
        TS_ASSERT_THROWS_NOTHING(dout = dserver->command_inout("ZmqEventSubscriptionChange", din));

        // extract and check the zmq ports for events and heartbeat
    		const DevVarLongStringArray *result;
        dout >> result;
        std::string heartbeat_endpoint = string(result->svalue[0]);
        std::string event_endpoint = string(result->svalue[1]);
        coutv << endl << "test_device_server_zmq_event_port_configured: finished" << endl << endl;
        coutv << "Heartbeat endpoint: " << heartbeat_endpoint << endl;
        coutv << "Event endpoint: " << event_endpoint << endl;

        pos = heartbeat_endpoint.rfind(':');
        heartbeat_port_str = heartbeat_endpoint.substr(pos + 1);
        TS_ASSERT_EQUALS(heartbeat_port, heartbeat_port_str);
        pos = event_endpoint.rfind(':');
        event_port_str = event_endpoint.substr(pos + 1);
        TS_ASSERT_EQUALS(event_port, event_port_str);

        // Cleanup device proxies
    		delete device1;
    		delete dserver;
    }

//
// Ask the device server to subscribe to an event and then check the
// ZmqEventSubscriptionChange ["info"] results to see whether it has
// ephemeral event and heartbeat ports on the DS
//
    void test_device_server_zmq_event_port_ephemeral(void) {
        // Stop any previously running server first

        DeviceProxy *dserver;
        DeviceProxy *device1;

        try {
            // Stop any previously running server first
            CxxTest::TangoPrinter::kill_server();

            // Restart the device1 server without any port config
            coutv << endl << "test_device_server_zmq_event_port_ephemeral: relaunching DevTEst/test with ephemeral zmq ports" << endl;
            CxxTest::TangoPrinter::start_server(device1_instance_name);

            CxxTest::TangoPrinter::restore_set("test/debian8/10 started.");
      			device1 = new DeviceProxy(device1_name);
      			dserver = new DeviceProxy(dserver_name);
      			device1->ping();
      			dserver->ping();
        }
        catch (CORBA::Exception &e) {
            Except::print_exception(e);
            exit(-1);
        }

        // restart the device proxies
  			device1 = new DeviceProxy(device1_name);
  			dserver = new DeviceProxy(dserver_name);
  			device1->ping();
  			dserver->ping();

        coutv << endl << "test_device_server_zmq_event_port_ephemeral: new DeviceProxy(" << device1->name() +"[" + device1_name +"]" << ") returned" << endl;

        // setup a subscription so that the event port is woken up
        EventCallback<Tango::EventData> callback{};
        std::string attribute_name = "event_change_tst";
        TS_ASSERT_THROWS_NOTHING(device1->subscribe_event(
            attribute_name,
            Tango::USER_EVENT,
            &callback));
        coutv << endl << "test_device_server_zmq_event_port_ephemeral: sending IOPushEvent" << endl;
        TS_ASSERT_THROWS_NOTHING(device1->command_inout("IOPushEvent"));
        Tango_sleep(2);

        // query the admin device so that we get the zmq port specifications
        coutv << endl << "test_device_server_zmq_event_port_ephemeral: sending ZmqEventSubscriptionChange" << endl << endl;
    		DeviceData din,dout;
				vector<string> din_info;
				din_info.push_back("info");
				din << din_info;
        TS_ASSERT_THROWS_NOTHING(dout = dserver->command_inout("ZmqEventSubscriptionChange", din));

        // extract and check the zmq ports for events and heartbeat
    		const DevVarLongStringArray *result;
        dout >> result;
        std::string heartbeat_endpoint = string(result->svalue[0]);
        std::string event_endpoint = string(result->svalue[1]);
        coutv << endl << "test_device_server_zmq_event_port_ephemeral: finished" << endl << endl;
        coutv << "Heartbeat endpoint: " << heartbeat_endpoint << endl;
        coutv << "Event endpoint: " << event_endpoint << endl;

        // Check that the ports are not equal to the fixed ones ie. ephemeral
        pos = heartbeat_endpoint.rfind(':');
        heartbeat_port_str = heartbeat_endpoint.substr(pos + 1);
        TS_ASSERT(heartbeat_port != heartbeat_port_str);
        pos = event_endpoint.rfind(':');
        event_port_str = event_endpoint.substr(pos + 1);
        TS_ASSERT(event_port != event_port_str);

        // Cleanup device proxies
    		delete device1;
    		delete dserver;
    }
};

#undef cout
#endif // ServerConfigureEventTestSuite_h
