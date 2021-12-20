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

        TS_ASSERT_THROWS_NOTHING(
            try {
                // Stop any previously running server first
                CxxTest::TangoPrinter::kill_server();

                // Restart the test server with env vars for the fixed ports
                std::string env = "TANGO_ZMQ_EVENT_PORT="+event_port+" TANGO_ZMQ_HEARTBEAT_PORT="+heartbeat_port+" ";
                coutv << endl << "test_device_server_zmq_event_port_configured: relaunching DevTest/test with fixed zmq ports - " << env + Tango::kStartServerCmd + device1_instance_name << endl;
                system((env + Tango::kStartServerCmd + device1_instance_name).c_str());

                CxxTest::TangoPrinter::restore_set("test/debian8/10 started.");
            }
            catch (CORBA::Exception &e) {
                Except::print_exception(e);
            }
        );

        // connect and ping device1
        bool res_ping_device1 = true;
        TS_ASSERT_THROWS_NOTHING(
            try {
                device1 = new DeviceProxy(device1_name);
                res_ping_device1 = device1->ping();
            }
            catch (CORBA::Exception &e) {
                coutv << endl << "test_device_server_zmq_event_port_configured: device1 corba error" << endl;
                Except::print_exception(e);
            }
        );
        TS_ASSERT(res_ping_device1);

        // connect and ping admin device
        bool res_ping_dserver = true;
        TS_ASSERT_THROWS_NOTHING(
            try {
                dserver = new DeviceProxy(dserver_name);
                res_ping_dserver = dserver->ping();
            }
            catch (CORBA::Exception &e) {
                coutv << endl << "test_device_server_zmq_event_port_configured: dserver corba error" << endl;
                Except::print_exception(e);
            }
        );
        TS_ASSERT(res_ping_dserver);

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
// Specify an invalid event port specification - expect an error
//
    void test_device_server_zmq_event_invalid_event_port_configured(void) {

        DeviceProxy *dserver;
        DeviceProxy *device1;

        TS_ASSERT_THROWS_NOTHING(
            try {
                // Stop any previously running server first
                CxxTest::TangoPrinter::kill_server();

                // Restart the test server with env vars for the fixed ports
                std::string env = "TANGO_ZMQ_EVENT_PORT=XXXX  TANGO_ZMQ_HEARTBEAT_PORT="+heartbeat_port+" ";
                coutv << endl << "test_device_server_zmq_event_invalid_port_configured: relaunching DevTest/test with fixed zmq ports - " << env + Tango::kStartServerCmd + device1_instance_name << endl;
                system((env + Tango::kStartServerCmd + device1_instance_name).c_str());

                CxxTest::TangoPrinter::restore_set("test/debian8/10 started.");
            }
            catch (CORBA::Exception &e) {
                Except::print_exception(e);
            }
        );

        // wait a bit for the server to properly start
        Tango_sleep(2);

        // connect and ping device1
        bool res_ping_device1 = true;
        TS_ASSERT_THROWS_NOTHING(
            try {
                device1 = new DeviceProxy(device1_name);
                res_ping_device1 = device1->ping();
            }
            catch (CORBA::Exception &e) {
                coutv << endl << "test_device_server_zmq_event_invalid_port_configured: device1 corba error" << endl;
                Except::print_exception(e);
            }
        );
        TS_ASSERT(res_ping_device1);

        // connect and ping admin device
        bool res_ping_dserver = true;
        TS_ASSERT_THROWS_NOTHING(
            try {
                dserver = new DeviceProxy(dserver_name);
                res_ping_dserver = dserver->ping();
            }
            catch (CORBA::Exception &e) {
                coutv << endl << "test_device_server_zmq_event_invalid_port_configured: dserver corba error" << endl;
                Except::print_exception(e);
            }
        );
        TS_ASSERT(res_ping_dserver);

        // setup a subscription so that the event port is woken up - check it that it fails
        coutv << endl << "test_device_server_zmq_event_invalid_port_configured: new DeviceProxy(" << device1->name() +"[" + device1_name +"]" << ") returned" << endl;

        EventCallback<Tango::EventData> callback{};
        std::string attribute_name = "event_change_tst";
        TS_ASSERT_THROWS_ASSERT(device1->subscribe_event(
            attribute_name,
            Tango::USER_EVENT,
            &callback),
            Tango::DevFailed &e,
            TS_ASSERT(e.errors.length() >= 1);
            coutv << endl << "test_device_server_zmq_event_invalid_port_configured: print_exception:" << endl;
            Except::print_exception(e);
            // The errors are completely unpredictable!!!!
            // TS_ASSERT(e.errors.length() >= 3);
            // // TS_ASSERT_EQUALS(std::string(e.errors[0].reason.in()), "API_ZmqInitFailed");
            // // TS_ASSERT_EQUALS(std::string(e.errors[1].reason.in()), "API_CommandFailed");
            // // TS_ASSERT_EQUALS(std::string(e.errors[2].reason.in()), "API_DSFailedRegisteringEvent");
            // TS_ASSERT_EQUALS(std::string(e.errors[0].reason.in()), "API_CorbaException");
            // TS_ASSERT_EQUALS(std::string(e.errors[1].reason.in()), "API_CantConnectToDevice");
            // TS_ASSERT_EQUALS(std::string(e.errors[2].reason.in()), "API_CommandFailed");
            // TS_ASSERT_EQUALS(std::string(e.errors[3].reason.in()), "API_DSFailedRegisteringEvent");
        );

        // Cleanup device proxies
        delete device1;
        delete dserver;
    }


//
// Specify an invalid heartbeat port specification - expect an error
//
    void test_device_server_zmq_event_invalid_heartbeat_port_configured(void) {

        DeviceProxy *device1;

        TS_ASSERT_THROWS_NOTHING(
            try {
                // Stop any previously running server first
                CxxTest::TangoPrinter::kill_server();

                // Restart the test server with env vars for the fixed ports
                std::string env = "TANGO_ZMQ_EVENT_PORT="+event_port+" TANGO_ZMQ_HEARTBEAT_PORT=YYYY ";
                coutv << endl << "test_device_server_zmq_event_invalid_heartbeat_port_configured: relaunching DevTest/test with fixed zmq ports - " << env + Tango::kStartServerCmd + device1_instance_name << endl;
                system((env + Tango::kStartServerCmd + device1_instance_name).c_str());

                CxxTest::TangoPrinter::restore_set("test/debian8/10 started.");
            }
            catch (CORBA::Exception &e) {
                Except::print_exception(e);
            }
        );

        // wait a bit for the server to properly start
        Tango_sleep(2);

        // try and ping the device - this should fail because of bad heartbeat config
        device1 = new DeviceProxy(device1_name);
        TS_ASSERT_THROWS_ASSERT(
            device1->ping(),
            Tango::DevFailed &e,
            TS_ASSERT(e.errors.length() >= 1);
            coutv << endl << "test_device_server_zmq_event_invalid_heartbeat_port_configured: print_exception:" << endl;
            Except::print_exception(e);
            // The errors are completely unpredictable!!!!
            // TS_ASSERT_EQUALS(std::string(e.errors[0].reason.in()), "API_CorbaException");
            // TS_ASSERT_EQUALS(std::string(e.errors[1].reason.in()), "API_CantConnectToDevice");
            // // TS_ASSERT_EQUALS(std::string(e.errors[0].reason.in()), "API_DeviceNotExported");
        );

        // Cleanup device proxies
        delete device1;
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

        TS_ASSERT_THROWS_NOTHING(

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
            }
        );

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
        // and that they are in the ephemeral range - default 32768 to 60999
        pos = heartbeat_endpoint.rfind(':');
        heartbeat_port_str = heartbeat_endpoint.substr(pos + 1);
        TS_ASSERT(heartbeat_port != heartbeat_port_str);
        int heartbeat_port_int = std::stoi(heartbeat_port_str);
        TS_ASSERT(heartbeat_port_int >= 32768 && heartbeat_port_int <= 60999)
        pos = event_endpoint.rfind(':');
        event_port_str = event_endpoint.substr(pos + 1);
        TS_ASSERT(event_port != event_port_str);
        int event_port_int = std::stoi(event_port_str);
        TS_ASSERT(event_port_int >= 32768 && event_port_int <= 60999)

        // Cleanup device proxies
        delete device1;
        delete dserver;
    }
};

#undef cout
#endif // ServerConfigureEventTestSuite_h
