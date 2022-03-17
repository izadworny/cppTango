/*
 * example of a client using the TANGO device api.
 */

#include <tango.h>
#include <assert.h>

#define coutv         \
  if(verbose == true) \
  cout

using namespace Tango;

bool verbose = false;

class MyCallBack : public CallBack
{
public:
  MyCallBack()
      : cb_executed(0)
  {
  }

  virtual void cmd_ended(CmdDoneEvent *);

  long cb_executed;
};

void MyCallBack::cmd_ended(CmdDoneEvent *cmd)
{
  coutv << "In cmd_ended method for device " << cmd->device->dev_name() << std::endl;
  coutv << "Command = " << cmd->cmd_name << std::endl;
  short l;
  cmd->argout >> l;
  coutv << "Command result = " << l << std::endl;

  cb_executed++;
}

int main(int argc, char **argv)
{
  DeviceProxy *device, *device2;

  if((argc < 3) || (argc > 4))
  {
    cout << "usage: asyn_cb2 <device1> <device2> [-v]" << std::endl;
    exit(-1);
  }

  std::string device1_name = argv[1];
  std::string device2_name = argv[2];

  if(argc == 4)
  {
    if(strcmp(argv[3], "-v") == 0)
      verbose = true;
  }

  try
  {
    device  = new DeviceProxy(device1_name);
    device2 = new DeviceProxy(device2_name);
  }
  catch(CORBA::Exception &e)
  {
    Except::print_exception(e);
    exit(1);
  }

  MyCallBack cb_dev1, cb_dev2;
  try
  {
    // Send commands to check asynchronous callbacks

    DeviceData din, dout;
    std::vector<short> send;
    send.push_back(4);
    send.push_back(1);
    din << send;

    device2->command_inout_asynch("IOShortSleep", din, cb_dev2);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);

    //
    // Wait for replies
    //

    long nb_replies;
    nb_replies = ApiUtil::instance()->pending_asynch_call(CALL_BACK);
    coutv << nb_replies << " request arrived" << std::endl;
    assert(nb_replies == 3);

    //
    // Fire callbacks only for one device
    //

    coutv << "Waiting for all replies for device" << std::endl;
    device->get_asynch_replies(0);

    assert(cb_dev1.cb_executed == 2);
    assert(cb_dev2.cb_executed == 0);

    nb_replies = ApiUtil::instance()->pending_asynch_call(CALL_BACK);
    coutv << nb_replies << " request arrived" << std::endl;

    //
    // Fire callback for the second device
    //

    device2->get_asynch_replies(500);

    assert(cb_dev1.cb_executed == 2);
    assert(cb_dev2.cb_executed == 1);

    cout << "   Callback fired by device --> OK" << std::endl;

    nb_replies = ApiUtil::instance()->pending_asynch_call(CALL_BACK);
    coutv << nb_replies << " request there" << std::endl;

    //
    // Check get_asynch_replies without any replies
    //

    device2->get_asynch_replies(200);
    device->get_asynch_replies(0);
    assert(cb_dev1.cb_executed == 2);
    assert(cb_dev2.cb_executed == 1);

    cout << "   Fire callabck without replies --> OK" << std::endl;

    //
    // Send commands
    //

    cb_dev1.cb_executed = 0;
    cb_dev2.cb_executed = 0;

    device2->command_inout_asynch("IOShortSleep", din, cb_dev2);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);

    int nb_not_arrived = 0;
    while(cb_dev1.cb_executed != 2)
    {
      try
      {
        device->get_asynch_replies(300);
      }
      catch(AsynReplyNotArrived &)
      {
        nb_not_arrived++;
        coutv << "Command not yet arrived" << std::endl;
      }
    }
    assert(nb_not_arrived >= 2);

    device2->get_asynch_replies(0);
    cout << "   Wait for device callback to be executed with timeout --> OK" << std::endl;

    //
    // Send commands
    //

    cb_dev1.cb_executed = 0;
    cb_dev2.cb_executed = 0;

    device2->command_inout_asynch("IOShortSleep", din, cb_dev2);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);

    coutv << "Waiting for replies" << std::endl;
    Tango_sleep(5);

    nb_replies = ApiUtil::instance()->pending_asynch_call(CALL_BACK);
    coutv << nb_replies << " request there" << std::endl;
    assert(nb_replies == 3);

    //
    // Fire callbacks for all devices
    //

    ApiUtil::instance()->get_asynch_replies(0);

    nb_replies = ApiUtil::instance()->pending_asynch_call(CALL_BACK);
    coutv << nb_replies << " request there" << std::endl;
    assert(nb_replies == 0);
    assert(cb_dev1.cb_executed == 2);
    assert(cb_dev2.cb_executed == 1);

    cout << "   Fire all callbacks at once when all already there --> OK" << std::endl;
    //
    // Send commands
    //

    cb_dev1.cb_executed = 0;
    cb_dev2.cb_executed = 0;

    device2->command_inout_asynch("IOShortSleep", din, cb_dev2);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);

    //
    // Wait for callbacks
    //

    ApiUtil::instance()->get_asynch_replies(0);

    nb_replies = ApiUtil::instance()->pending_asynch_call(CALL_BACK);
    coutv << nb_replies << " request there" << std::endl;
    assert(nb_replies == 0);
    assert(cb_dev1.cb_executed == 2);
    assert(cb_dev2.cb_executed == 1);

    cout << "   Wait for all callbacks to be executed --> OK" << std::endl;

    //
    // Send commands
    //

    cb_dev1.cb_executed = 0;
    cb_dev2.cb_executed = 0;

    device2->command_inout_asynch("IOShortSleep", din, cb_dev2);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);
    device->command_inout_asynch("IOShortSleep", din, cb_dev1);

    //
    // Wait for callback to be executed with Timeout
    //

    nb_not_arrived = 0;
    while(cb_dev1.cb_executed != 2)
    {
      try
      {
        ApiUtil::instance()->get_asynch_replies(300);
      }
      catch(AsynReplyNotArrived &)
      {
        nb_not_arrived++;
        coutv << "Command not yet arrived" << std::endl;
      }
    }
    assert(nb_not_arrived >= 2);
    assert(cb_dev1.cb_executed == 2);
    assert(cb_dev2.cb_executed == 1);

    cout << "   Wait for callback to be executed with timeout --> OK" << std::endl;
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
  delete device2;

  return 0;
}
