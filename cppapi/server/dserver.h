//=============================================================================
//
// file :               DServer.h
//
// description :        Include for the DServer class. This class implements
//                      all the commands which are available for device
//			of the DServer class. There is one device of the
//			DServer class for each device server process
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
//=============================================================================

#ifndef _DSERVER_H
#define _DSERVER_H

#include <tango.h>
#include "event_subscription_state.h"
#include <tango_clock.h>

namespace Tango
{

struct MulticastParameters
{
    std::string endpoint;
    int rate;
    int recovery_ivl;
};

//=============================================================================
//
//			The DServer class
//
// description :	Class to implement all data members and commands for
//			device of the DServer class
//
//=============================================================================

typedef Tango::DeviceClass *(*Cpp_creator_ptr)(const char *);
typedef void (*ClassFactoryFuncPtr)(DServer *);


class DServer: public TANGO_BASE_CLASS
{
public :
	DServer(DeviceClass *,const char *,const char *,Tango::DevState,const char *);
	~DServer();

	Tango::DevVarStringArray *query_class();
	Tango::DevVarStringArray *query_device();
	Tango::DevVarStringArray *query_sub_device();
	void kill();
	void restart(const std::string &);
	void restart_server();
	Tango::DevVarStringArray *query_class_prop(std::string &);
	Tango::DevVarStringArray *query_dev_prop(std::string &);

	Tango::DevVarStringArray *polled_device();
	Tango::DevVarStringArray *dev_poll_status(const std::string &);
	void add_obj_polling(const Tango::DevVarLongStringArray *,bool with_db_upd = true,int delta_ms = 0);
	void upd_obj_polling_period(const Tango::DevVarLongStringArray *,bool with_db_upd = true);
	void rem_obj_polling(const Tango::DevVarStringArray *,bool with_db_upd = true);
	void stop_polling();
	void start_polling();
	void start_polling(PollingThreadInfo *);
	void add_event_heartbeat();
	void rem_event_heartbeat();

	void lock_device(const Tango::DevVarLongStringArray *);
	Tango::DevLong un_lock_device(const Tango::DevVarLongStringArray *);
	void re_lock_devices(const Tango::DevVarStringArray *);
	Tango::DevVarLongStringArray *dev_lock_status(Tango::ConstDevString);

	Tango::DevLong event_subscription_change(const Tango::DevVarStringArray *);
	Tango::DevVarLongStringArray *zmq_event_subscription_change(const Tango::DevVarStringArray *);
	void event_confirm_subscription(const Tango::DevVarStringArray *);

	void delete_devices();

	void add_logging_target (const Tango::DevVarStringArray *argin);
	void remove_logging_target (const Tango::DevVarStringArray *argin);
	Tango::DevVarStringArray* get_logging_target (const std::string& dev_name);
	void set_logging_level (const Tango::DevVarLongStringArray *argin);
	Tango::DevVarLongStringArray* get_logging_level (const Tango::DevVarStringArray *argin);
	void stop_logging (void);
	void start_logging (void);

	std::string &get_process_name() {return process_name;}
	std::string &get_personal_name() {return instance_name;}
	std::string &get_instance_name() {return instance_name;}
	std::string &get_full_name() {return full_name;}
	std::string &get_fqdn() {return fqdn;}
	bool get_heartbeat_started() {return heartbeat_started;}
	void set_heartbeat_started(bool val) {heartbeat_started = val;}

	std::vector<DeviceClass *> &get_class_list() {return class_list;}
	virtual void init_device();

	virtual void server_init_hook();

	unsigned long get_poll_th_pool_size() {return polling_th_pool_size;}
	void set_poll_th_pool_size(unsigned long val) {polling_th_pool_size = val;}
	bool get_opt_pool_usage() {return optimize_pool_usage;}
	std::vector<std::string> get_poll_th_conf() {return polling_th_pool_conf;}

	void check_lock_owner(DeviceImpl *,const char *,const char *);
	void check_upd_authorized(DeviceImpl *,int,PollObjType,const std::string &);

	TANGO_IMP_EXP static void register_class_factory(ClassFactoryFuncPtr f_ptr) {class_factory_func_ptr = f_ptr;}
	void _add_class(DeviceClass *dc) {this->add_class(dc);}
	void _create_cpp_class(const char *c1,const char *c2) {this->create_cpp_class(c1,c2);}

	void mcast_event_for_att(const std::string &,const std::string &,std::vector<std::string> &);

	ServerEventSubscriptionState get_event_subscription_state();
	void set_event_subscription_state(const ServerEventSubscriptionState&);

	void mem_devices_interface(std::map<std::string,DevIntr> &);
	void changed_devices_interface(std::map<std::string,DevIntr> &);

    bool is_polling_bef_9_def() {return polling_bef_9_def;}
    bool get_polling_bef_9() {return polling_bef_9;}

	friend class NotifdEventSupplier;
	friend class ZmqEventSupplier;

protected :
	std::string							process_name;
	std::string							instance_name;
	std::string							full_name;
	std::string 							fqdn;

	std::vector<DeviceClass *>			class_list;

	time_t							last_heartbeat;
	time_t                          last_heartbeat_zmq;
	bool							heartbeat_started;

	unsigned long					polling_th_pool_size;
	std::vector<std::string>					polling_th_pool_conf;
	bool							optimize_pool_usage;

	static ClassFactoryFuncPtr 		class_factory_func_ptr;

private:
#if ((defined _TG_WINDOWS_) && (defined TANGO_HAS_DLL) && !(defined _TANGO_LIB))
	__declspec(dllexport) void class_factory();
#else
	void class_factory();
#endif
	void add_class(DeviceClass *);
	void create_cpp_class(const char *,const char *);
	void get_dev_prop(Tango::Util *);
	void event_subscription(
		DeviceImpl &device,
		const std::string &obj_name,
		const std::string &action,
		const std::string &event,
		ChannelType channel_type,
		int client_lib_version);
	MulticastParameters get_multicast_parameters(DeviceImpl& device, const std::string& object_name, const std::string& event);
	void store_subscribed_client_info(
		DeviceImpl& device, const std::string& object_name, const std::string &event_name, int client_lib_version);
	void get_event_misc_prop(Tango::Util *);
	bool is_event_name(const std::string &);
	bool is_ip_address(const std::string &);

	std::vector<std::string>	mcast_event_prop;

	DevLong         mcast_hops;
	DevLong         mcast_rate;
	DevLong         mcast_ivl;
	DevLong         zmq_pub_event_hwm;
	DevLong         zmq_sub_event_hwm;

	bool            polling_bef_9_def;
	bool            polling_bef_9;
};

class KillThread: public omni_thread
{
public:

	void *run_undetached(void *);
	void start() {start_undetached();}
};

class ServRestartThread: public omni_thread
{
public:
	ServRestartThread(DServer *dev):omni_thread(dev) {}

	void run(void *);
};

struct Pol
{
	PollObjType type;
	PollClock::duration upd;
	std::string name;
};


/******************************************************************************
 *
 *			Some inline methods
 *
 ******************************************************************************/

inline bool DServer::is_event_name(const std::string &str)
{
	if (count(str.begin(),str.end(),'/') != 3)
		return false;
	if (count(str.begin(),str.end(),'.') != 1)
		return false;
	return true;
}

inline bool DServer::is_ip_address(const std::string &str)
{
	if (count(str.begin(),str.end(),'.') != 3)
		return false;
	return true;
}

} // End of namespace Tango

#endif
