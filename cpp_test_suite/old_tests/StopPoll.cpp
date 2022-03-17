/*
 * example of a client using the TANGO device api.
 */

#include <tango.h>

using namespace Tango;
using namespace std;

void stop_poll_att_no_except(DeviceProxy *, const char *);
void stop_poll_cmd_no_except(DeviceProxy *, const char *);

int main(int argc, char **argv)
{
  DeviceProxy *device      = NULL;
  DeviceProxy *kill_device = NULL;
  string device_name;
  string kill_device_name;

  for(int i = 1; i < argc; ++i)
  {
    if(string(argv[i]) == "--kill")
    {
      if(i + 1 < argc)
      {
        kill_device_name = argv[++i]; // Increment 'i' so we don't get the
                                      // argument as the next argv[i].
      }
      else
      {
        cerr << "--kill option requires one argument." << endl;
        exit(-1);
      }
    }
    else
    {
      device_name = argv[i];
    }
  }

  //	cout << "Device name = " << device_name << endl;
  //	cout << "Kill device name = " << kill_device_name << endl;

  try
  {
    if(device_name.empty() == false)
      device = new DeviceProxy(device_name);
  }
  catch(Tango::DevFailed &e)
  {
    device_name.clear();
  }

  try
  {
    if(kill_device_name.empty() == false)
      kill_device = new DeviceProxy(kill_device_name);
  }
  catch(CORBA::Exception &e)
  {
    kill_device_name.clear();
  }

  //	cout << endl << "new DeviceProxy(" << device->name() << ") returned" <<
  // endl << endl;

  if(kill_device_name.empty() == false)
  {
    try
    {
      string adm_name = kill_device->adm_name();
      DeviceProxy adm_dev(adm_name);
      adm_dev.command_inout("kill");

      delete kill_device;
    }
    catch(Tango::DevFailed &e)
    {
    }
  }

  if(device_name.empty() == false)
  {
    stop_poll_cmd_no_except(device, "IOPollStr1");
    stop_poll_cmd_no_except(device, "IOPollArray2");
    stop_poll_cmd_no_except(device, "IOExcept");
    stop_poll_cmd_no_except(device, "OEncoded");
    stop_poll_cmd_no_except(device, "State");
    stop_poll_cmd_no_except(device, "Status");

    stop_poll_att_no_except(device, "PollLong_attr");
    stop_poll_att_no_except(device, "PollString_spec_attr");
    stop_poll_att_no_except(device, "attr_wrong_type");

    stop_poll_att_no_except(device, "Long64_attr_rw");
    stop_poll_att_no_except(device, "ULong_spec_attr_rw");
    stop_poll_att_no_except(device, "ULong64_attr_rw");
    stop_poll_att_no_except(device, "State_spec_attr_rw");
    stop_poll_att_no_except(device, "Encoded_attr");

    stop_poll_att_no_except(device, "event_change_tst");
    stop_poll_att_no_except(device, "event64_change_tst");
    stop_poll_att_no_except(device, "short_attr");
    stop_poll_att_no_except(device, "slow_actuator");
    stop_poll_att_no_except(device, "fast_actuator");

    delete device;
  }
}

void stop_poll_att_no_except(DeviceProxy *dev, const char *att_name)
{
  try
  {
    dev->stop_poll_attribute(att_name);
  }
  catch(Tango::DevFailed &)
  {
  }
  catch(CORBA::Exception &e)
  {
    Except::print_exception(e);
    exit(-1);
  }
}

void stop_poll_cmd_no_except(DeviceProxy *dev, const char *cmd_name)
{
  try
  {
    dev->stop_poll_command(cmd_name);
  }
  catch(Tango::DevFailed &)
  {
  }
  catch(CORBA::Exception &e)
  {
    Except::print_exception(e);
    exit(-1);
  }
}
