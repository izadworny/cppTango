//
// Created by ingvord on 12/14/16.
//
#ifndef StatelessSubTestSuite_h
#define StatelessSubTestSuite_h

#include <thread>
#include "cxx_common.h"

#define coutv         \
  if(verbose == true) \
  cout

#undef SUITE_NAME
#define SUITE_NAME StatelessSubTestSuite

class EventCallback : public Tango::CallBack
{
public:
  EventCallback() {}

  ~EventCallback() {}

  void push_event(Tango::EventData *ed)
  {
    cout << "In callback with error flag = " << std::boolalpha << ed->err << endl;
    if(ed->err)
    {
      cb_err++;
      cout << "Error: " << ed->errors[0].reason << endl;
    }
    else
    {
      cb_executed++;
    }
  }

  int cb_executed;
  int cb_err;
};

class StatelessSubTestSuite : public CxxTest::TestSuite
{
protected:
  DeviceProxy *device2;
  string device2_name, device1_instance_name, device2_instance_name;
  bool verbose;
  EventCallback eventCallback;

public:
  SUITE_NAME()
      : device2_instance_name{"test2"},
        eventCallback{}
  {
    //
    // Arguments check -------------------------------------------------
    //

    device2_name = CxxTest::TangoPrinter::get_param("device20");

    verbose = CxxTest::TangoPrinter::is_param_defined("verbose");

    CxxTest::TangoPrinter::validate_args();

    //
    // Initialization --------------------------------------------------
    //

    try
    {
      device2 = new DeviceProxy(device2_name);

      // sleep 24 &&  start_server "@INST_NAME@2" &
      thread(
          [this]()
          {
            Tango_sleep(24);
            CxxTest::TangoPrinter::start_server(device2_instance_name);
            CxxTest::TangoPrinter::restore_set("test2/debian8/20 started.");
          })
          .detach();
    }
    catch(CORBA::Exception &e)
    {
      Except::print_exception(e);
      exit(-1);
    }
  }

  virtual ~SUITE_NAME()
  {
    if(CxxTest::TangoPrinter::is_restore_set("test2/debian8/20 started."))
      CxxTest::TangoPrinter::kill_server();

    CxxTest::TangoPrinter::start_server("test");

    delete device2;
  }

  static SUITE_NAME *createSuite() { return new SUITE_NAME(); }

  static void destroySuite(SUITE_NAME *suite) { delete suite; }

  //
  // Tests -------------------------------------------------------
  //

  //
  // Subscribe to event with stateless flag set
  //
  void test_unsubscribe_from_stateless_event(void)
  {
    string att_name("event_change_tst");

    int eventID = 0;
    const vector<string> filters;
    eventCallback.cb_executed = 0;
    eventCallback.cb_err      = 0;

    TS_ASSERT_THROWS_NOTHING(
        eventID = device2->subscribe_event(att_name, Tango::CHANGE_EVENT, &eventCallback, filters, true));

    Tango_sleep(6);

    TS_ASSERT_THROWS_NOTHING(device2->unsubscribe_event(eventID));
  }

  //
  // Re-subscribe
  //
  void test_re_subscribe_and_check(void)
  {
    string att_name("event_change_tst");
    TS_ASSERT_THROWS_NOTHING(device2->subscribe_event(att_name, Tango::CHANGE_EVENT, &eventCallback, true));

    //
    // Wait for connection and event
    //

    Tango_sleep(40);

    //
    // Check error and connection
    //

    coutv << "cb err = " << eventCallback.cb_err << endl;
    coutv << "cb executed = " << eventCallback.cb_executed << endl;

    TS_ASSERT_LESS_THAN_EQUALS(1, eventCallback.cb_err);
    TS_ASSERT_LESS_THAN_EQUALS(1, eventCallback.cb_executed);
  }
};

#undef cout
#endif // StatelessSubTestSuite_h
