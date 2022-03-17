/*
 * example of a client using the TANGO device api.
 */

#include <tango.h>
#include <assert.h>

#ifdef WIN32
  #include <sys/timeb.h>
  #include <process.h>
#else
  #include <sys/time.h>
  #include <unistd.h>
#endif

#define coutv         \
  if(verbose == true) \
  cout

using namespace Tango;

bool verbose = false;

class SlowCallBack : public Tango::CallBack
{
  void push_event(Tango::EventData *);

public:
  int cb_executed;
  int cb_err;
  int old_sec, old_usec;
  int delta_msec;
  short val;
  AttrQuality qua;
};

class FastCallBack : public Tango::CallBack
{
  void push_event(Tango::EventData *);

public:
  int cb_executed;
  int cb_err;
  int old_sec, old_usec;
  int delta_msec;
  double val;
  AttrQuality qua;
};

void SlowCallBack::push_event(Tango::EventData *event_data)
{
  std::vector<short> value;
  struct timeval now_timeval;

#ifdef WIN32
  struct _timeb before_win;
  _ftime(&before_win);
  now_timeval.tv_sec  = (unsigned long) before_win.time;
  now_timeval.tv_usec = (long) before_win.millitm * 1000;
#else
  gettimeofday(&now_timeval, NULL);
#endif
  coutv << "date : tv_sec = " << now_timeval.tv_sec;
  coutv << ", tv_usec = " << now_timeval.tv_usec << std::endl;

  int delta_s = now_timeval.tv_sec - old_sec;
  if(delta_s == 0)
    delta_msec = (now_timeval.tv_usec - old_usec) / 1000;
  else
  {
    delta_msec = (now_timeval.tv_usec + (1000000 - old_usec)) / 1000;
    if(delta_s > 1)
      delta_msec = delta_msec + ((delta_s - 1) * 1000);
  }
  old_sec  = now_timeval.tv_sec;
  old_usec = now_timeval.tv_usec;

  coutv << "delta_msec = " << delta_msec << std::endl;

  cb_executed++;
  try
  {
    coutv << "SlowCallBack::push_event(): called attribute " << event_data->attr_name << " event " << event_data->event
          << "\n";
    qua = event_data->attr_value->get_quality();
    if((!event_data->err) && (qua != ATTR_INVALID))
    {
      *(event_data->attr_value) >> value;
      val = value[0];
      coutv << "Callback value " << val << std::endl;
    }
    else
    {
      coutv << "Error send to callback" << std::endl;
      //			Tango::Except::print_error_stack(event_data->errors);
    }
  }
  catch(...)
  {
    coutv << "SlowCallBack::push_event(): could not extract data !\n";
  }
}

void FastCallBack::push_event(Tango::EventData *event_data)
{
  std::vector<double> value;
  struct timeval now_timeval;

#ifdef WIN32
  struct _timeb before_win;
  _ftime(&before_win);
  now_timeval.tv_sec  = (unsigned long) before_win.time;
  now_timeval.tv_usec = (long) before_win.millitm * 1000;
#else
  gettimeofday(&now_timeval, NULL);
#endif
  coutv << "date : tv_sec = " << now_timeval.tv_sec;
  coutv << ", tv_usec = " << now_timeval.tv_usec << std::endl;

  int delta_s = now_timeval.tv_sec - old_sec;
  if(delta_s == 0)
    delta_msec = (now_timeval.tv_usec - old_usec) / 1000;
  else
  {
    delta_msec = (now_timeval.tv_usec + (1000000 - old_usec)) / 1000;
    if(delta_s > 1)
      delta_msec = delta_msec + ((delta_s - 1) * 1000);
  }
  old_sec  = now_timeval.tv_sec;
  old_usec = now_timeval.tv_usec;

  coutv << "delta_msec = " << delta_msec << std::endl;

  cb_executed++;
  try
  {
    coutv << "FastCallBack::push_event(): called attribute " << event_data->attr_name << " event " << event_data->event
          << "\n";
    qua = event_data->attr_value->get_quality();
    if((!event_data->err) && (qua != ATTR_INVALID))
    {
      *(event_data->attr_value) >> value;
      val = value[0];
      coutv << "Callback value " << val << std::endl;
    }
    else
    {
      coutv << "Error send to callback" << std::endl;
      //			Tango::Except::print_error_stack(event_data->errors);
    }
  }
  catch(...)
  {
    coutv << "FastCallBack::push_event(): could not extract data !\n";
  }
}

int main(int argc, char **argv)
{
  DeviceProxy *device;

  if(argc == 1)
  {
    cout << "usage: %s device [-v]" << std::endl;
    exit(-1);
  }

  std::string device_name = argv[1];

  if(argc == 3)
  {
    if(strcmp(argv[2], "-v") == 0)
      verbose = true;
  }

  try
  {
    device = new DeviceProxy(device_name);
  }
  catch(CORBA::Exception &e)
  {
    Except::print_exception(e);
    exit(1);
  }

  coutv << std::endl << "new DeviceProxy(" << device->name() << ") returned" << std::endl << std::endl;

  try
  {
    std::string att_name("slow_actuator");

    //
    // Test set up (stop polling and clear abs_change and rel_change attribute
    // properties but restart device to take this into account)
    // Set the abs_change to 1
    //

    if(device->is_attribute_polled(att_name))
      device->stop_poll_attribute(att_name);
    {
      DbAttribute dba(att_name, device_name);
      DbData dbd;
      DbDatum a(att_name);
      a << (short) 2;
      dbd.push_back(a);
      dbd.push_back(DbDatum("abs_change"));
      dbd.push_back(DbDatum("rel_change"));
      dba.delete_property(dbd);

      dbd.clear();
      a << (short) 1;
      dbd.push_back(a);
      DbDatum ch("abs_change");
      ch << (short) 1;
      dbd.push_back(ch);
      dba.put_property(dbd);

      DeviceProxy adm_dev(device->adm_name().c_str());
      DeviceData di;
      di << device_name;
      adm_dev.command_inout("DevRestart", di);
    }

    delete device;
    device = new DeviceProxy(device_name);
    Tango_sleep(1);

    //
    // switch on the polling first!
    //
    device->poll_attribute(att_name, 250);

    //
    // Check that the attribute is now polled at 1000 mS
    //

    bool po = device->is_attribute_polled(att_name);
    coutv << "attribute polled : " << po << std::endl;
    assert(po == true);

    int poll_period = device->get_attribute_poll_period(att_name);
    coutv << "att polling period : " << poll_period << std::endl;
    assert(poll_period == 250);

    //
    // subscribe to a change event and filter only on a quality change
    //

    int eve_id;
    SlowCallBack cb;
    cb.cb_executed = 0;
    cb.cb_err      = 0;
    cb.old_sec = cb.old_usec = 0;

    std::vector<std::string> filters;
    filters.push_back("$quality == 1");
    eve_id = device->subscribe_event(att_name, Tango::CHANGE_EVENT, &cb, filters);

#ifdef _TG_WINDOWS_
    Sleep((DWORD) 250);
#else
    struct timespec to_wait, inter;
    to_wait.tv_sec  = 0;
    to_wait.tv_nsec = 250000000;
    nanosleep(&to_wait, &inter);
#endif

    //
    // Check that first point has been received
    //

    assert(cb.cb_executed == 1);
    assert(cb.qua == ATTR_VALID);
    cout << "   (Slow actuator) first point received --> OK" << std::endl;
    //
    // Write a value into the actuator
    //

    short new_val = 10;
    DeviceAttribute da(att_name, new_val);

    device->write_attribute(da);

#ifndef WIN32
    int rest = sleep(1);
    if(rest != 0)
      sleep(1);
#else
    Sleep(1000);
#endif

    //
    // Check that the event has been received
    //

    assert(cb.cb_executed == 2);
    assert(cb.qua == ATTR_CHANGING);
    cout << "   (Slow actuator) Event quality from VALID->CHANGING --> OK" << std::endl;

    //
    // Read value from device which should have quality factor still set to
    // CHANGING
    //

    DeviceAttribute da_r;
    device->set_source(DEV);
    da_r = device->read_attribute(att_name);
    device->set_source(CACHE_DEV);

    assert(da_r.get_quality() == ATTR_CHANGING);
    cout << "   (Slow actuator) Attribute quality still CHANGING after read "
            "--> OK"
         << std::endl;

    //
    // Wait for end of movement
    //

#ifndef WIN32
    rest = sleep(3);
    if(rest != 0)
      sleep(3);
#else
    Sleep(3000);
#endif

    //
    // Check that the event has been received
    //

    assert(cb.cb_executed == 3);
    assert(cb.qua == ATTR_VALID);
    cout << "   (Slow actuator) event quality from CHANGING->VALID --> OK" << std::endl;

    //
    // unsubscribe to the event
    //

    device->unsubscribe_event(eve_id);

    cout << "   (Slow actuator) unsubscribe_event --> OK" << std::endl;

    //---------------------------------------------------------------------------------
    //
    //			Now, check a fast actuator scan
    //
    //---------------------------------------------------------------------------------

    att_name = "fast_actuator";

    //
    // Test set up (stop polling and clear abs_change and rel_change attribute
    // properties but restart device to take this into account)
    // Set the abs_change to 1
    //

    if(device->is_attribute_polled(att_name))
      device->stop_poll_attribute(att_name);
    {
      DbAttribute dba(att_name, device_name);
      DbData dbd;
      DbDatum a(att_name);
      a << (short) 2;
      dbd.push_back(a);
      dbd.push_back(DbDatum("abs_change"));
      dbd.push_back(DbDatum("rel_change"));
      dba.delete_property(dbd);

      dbd.clear();
      a << (short) 1;
      dbd.push_back(a);
      DbDatum ch("abs_change");
      ch << (short) 1;
      dbd.push_back(ch);
      dba.put_property(dbd);

      DeviceProxy adm_dev(device->adm_name().c_str());
      DeviceData di;
      di << device_name;
      adm_dev.command_inout("DevRestart", di);
    }

    delete device;
    device = new DeviceProxy(device_name);
    Tango_sleep(1);

    //
    // switch on the polling first!
    //
    device->poll_attribute(att_name, 250);

    //
    // Check that the attribute is now polled at 100 mS
    //
    po = device->is_attribute_polled(att_name);
    coutv << "attribute polled : " << po << std::endl;
    assert(po == true);

    poll_period = device->get_attribute_poll_period(att_name);
    coutv << "att polling period : " << poll_period << std::endl;
    assert(poll_period == 250);

    //
    // subscribe to a change event and filter only on a quality change
    //

    FastCallBack fcb;
    fcb.cb_executed = 0;
    fcb.cb_err      = 0;
    fcb.old_sec = cb.old_usec = 0;

    std::vector<std::string> ffilters;
    ffilters.push_back("$quality == 1");
    eve_id = device->subscribe_event(att_name, Tango::CHANGE_EVENT, &fcb, ffilters);

    cout << "   (Fast actuator) subscribe_event --> OK" << std::endl;

#ifdef _TG_WINDOWS_
    Sleep((DWORD) 250);
#else
    to_wait.tv_sec  = 0;
    to_wait.tv_nsec = 250000000;
    nanosleep(&to_wait, &inter);
#endif

    //
    // Check that first point has been received
    //

    assert(fcb.cb_executed == 1);
    assert(fcb.qua == ATTR_VALID);
    cout << "   (Fast actuator) first point received --> OK" << std::endl;

    //
    // Write a value into the actuator
    //

    double val = 100.333;
    DeviceAttribute fda(att_name, val);

    device->write_attribute(fda);

    //  Only sleep a 100ms. The events should be fired immediately

#ifdef _TG_WINDOWS_
    Sleep((DWORD) 250);
#else
    to_wait.tv_sec  = 0;
    to_wait.tv_nsec = 250000000;
    nanosleep(&to_wait, &inter);
#endif

    //
    // Check that the events has been received
    //

    assert(fcb.cb_executed == 3);
    assert(fcb.qua == ATTR_VALID);
    cout << "   (Fast actuator) Two quality events received --> OK" << std::endl;

    //
    // unsubscribe to the event
    //

    device->unsubscribe_event(eve_id);
    cout << "   (Fast actuator) Unsubscribe_event --> OK" << std::endl;

    device->stop_poll_attribute(att_name);
    device->stop_poll_attribute("slow_actuator");
  }
  catch(Tango::DevFailed &e)
  {
    Except::print_exception(e);
    exit(-1);
  }
  catch(CORBA::Exception &ex)
  {
    Except::print_exception(ex);
    exit(-1);
  }

  delete device;

  return 0;
}
