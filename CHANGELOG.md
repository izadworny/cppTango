# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

## Unreleased

## [9.3.6] - 202X-XX-XX

## [9.3.5] - 2022-02-24

### Fixed

- Fix an artificial limit of six device classes per device server when using the FileDatabase ([#816][i-816], [!818][mr-818])
- Include all relevant classes in the C++ doxygen documentation ([!780][mr-780])
- No archive periodic events after polling restart ([#675][i-675], [!778][mr-778])
- Avoid requiring sed for compiling ([#823][i-823], [!824][mr-824])
- Allow compiling JPEG support with MMX extensions with old compilers again ([!828][mr-828])
- Fix wrong usage of omniORB object leading to crash ([#842][i-842], [!842][mr-842])
- Make compiling against alpine linux with musl libc work ([#767][i-767], [!841][mr-841])
- Fix deadlock when updating attribute configuration ([#409][i-409], [!863][mr-863])
- Increase timestamp precision in logging to microseconds ([#858][i-858], [!866][mr-866])
- Catch more exceptions in the PollThread to avoid crashes ([#691][i-691], [!873][mr-873])
- Make the ZeroMQ ports used for the event system configurable ([#880][i-880], [!893][mr-893])
- Windows: Split static/dynamic library building and fix cmake logic ([!827][mr-827])
- Automatically deploy the API documentation on git tag ([#545][i-545], [!909][mr-909])
- Linux: Add CMAKE_BUILD_TYPE options RelWithDebInfo/MinSizeRel ([#839][i-839], [!905][mr-905])
- Fix segfault on unsubscribing ([#843][i-843], [!852][mr-852])
- Fix crash when reading a forwarded State attribute ([#550][i-550], [!896][mr-896])

### Misc

- Switch from github to gitlab, dropping travis, add coverage support ([!825][mr-825])
- Fix exception safety issues when ZeroMQ ports are wrongly configured ([!900][mr-900])

## [9.3.4] - 2020-09-15

### Fixed
- Be compatible with all in-use cppzmq versions ([!530][mr-530], [!561][mr-561], [#273][i-273], [#499][i-499], [#535][i-535])
- Fix snprintf detection. Now the `LOG_*` macros work again ([!658][mr-658])
- Various spelling errors (found by lintian) ([!647][mr-647], [!621][mr-621], [!751][mr-751])
- CMake fixes regarding `CMAKE_BUILD_TYPE` logic ([!629][mr-629])
- Fix race conditions between polling threads and user threads pushing events ([!665][mr-665], [#511][i-511])
- Avoid undefined interface warning msvc ([!664][mr-664])
- travis.yml: remove deprecated skip_cleanup ([!683][mr-683])
- Fix memory leak in Attribute::get_att_device_class() ([!677][mr-677])
- Fix crash during alarm state evaluation if attribute value is not set ([#330][i-330], [!681][mr-681])
- Fix issue where unsubscribing in push_events led to API_EventTimeout ([#686][i-686], [!699][mr-699])
- Fix Debian7 Travis tests after move of Debian wheezy docker image to debian/eol ([!596][mr-596])
- Remove event blindness after device restart ([#496][i-496], [!702][mr-702])
- Include all relevant classes in the C++ doxygen documentation ([!780][mr-780])
- Fix connection with tango servers with version 9.3.0 in complex network setups ([!716][mr-716])
- Remove extra comma in struct declaration which trips up old compilers ([!709][mr-709])

### Added
- Document contributing process and use CODEOWNERS for easier review organization ([!639][mr-639])
- Add testing with debian buster ([!597][mr-597])
- Add CMake option to disable building the test suite ([!689][mr-689])
- Add CMake switch to disable building MMX ([!674][mr-674])
- appveyor.yml: Add inline deployment on tag push ([!690][mr-690])
- Speedup attribute performance in Multiattribute ([!738][mr-738])
- Respect the `BUILD_TESTING` cmake option for log4tango ([!743][mr-743])
- Only build either the static or the shared library ([#740][i-740], [!763][mr-763])

## [9.3.3] - 2019-03-29

### Added
- Add asyn_reconnection test ([!502][mr-502])
- Add .gitignore ([!522][mr-522])
- Add Visual Studio 2017 compatibility. Appveyor now builds for Visual Studio 2017 in addition to all the previously
supported versions ([!533][mr-533])

### Changed
- Replace CORBA::string_dup calls with Tango::string_dup ([!474][mr-474])
- Replace CORBA::string_free calls with Tango::string_free ([!539][mr-539])
- Make it possible to remove dynamic attributes from delete_device() by moving the location where the device is removed
from the device list ([!518][mr-518])
- Use python2 (if available) to run cxxtestgen ([!523][mr-523])
- Switch to Travis Xenial infrastructure ([!516][mr-516])
- Catch exceptions from unsubscribe_event() in DeviceProxy destructor ([!521][mr-521])
- Improve install documentation ([!529][mr-529])
- Install log4tango include files under *<install_prefix>*/include/tango/log4tango ([!517][mr-517])
- Update Doxygen documentation ([#532][i-532], [!544][mr-544])
- Update Debian 7 and Debian 8 Travis Docker files after Debian Wheezy end of life and jessie-updates removal
  ([#546][i-546], [!549][mr-549])

### Fixed
- Fix memory leak in get_device_property() on Windows when code compiled with Visual Studio 10 or older
([#439][i-439], [!488][mr-488])
- Fix cxx_fwd_att occasional failure ([#384][i-384], [#428][i-428], [!493][mr-493])
- Fix "Change event subscription blind to change events right after device server restart" issue
([#359][i-359], [!503][mr-503]).
**This bug fix may trigger the reception of 2 consecutive events having possibly the same attribute value
(but different timestamps) during the Device Server startup phase.**
- Fix client crash in case of asynchronous write_attribute executed immediately after reconnection ([!502][mr-502])
- Fix issue with dynamic attributes when a device is restarted on a device server exporting several devices which are
creating dynamic attributes ([#458][i-458], [!508][mr-508])
- Fix uncaught DevFailed exception in DeviceProxy destructor in cxx_stateless_subscription test case when using
ZMQ 4.2.3 ([#514][i-514]). Since [zeromq/libzmq@edb4ca1][libzmq-c-edb4ca1], peer disconnection triggers zmq_disconnect
on a zmq socket.
Further zmq_disconnect calls for such socket and endpoint will fail with errno set to ENOENT.
The patch provided in [!520][mr-520] ignores such failure.
- Fixed a broken dependency on generated Debian package ([5c74e8d][c-5c74e8d])
- Fix appveyor.yml after appveyor windows image update ([!537][mr-537])
- Fix event compatibility issues with device servers exporting Device_4 devices. Fix bug with Pipe, Data Ready and
Device Interface Change events. Fix event channel name issues in these different cases. ([#492][i-492], [!531][mr-531])
- Fix WAttribute::rollback() for DevUshort WRITE attributes ([#541][i-541], [!542][mr-542])

## [9.3.2] - 2018-09-12 [unstable]
### Added
- Add Tango::string_free ([!460][mr-460])
- Integrate Coveralls and Sonar ([#369][i-369], [!478][mr-478])

### Changed
- Tests: Split runner executable into several executables ([#275][i-275], [!476][mr-476]).
- Doxygen: upgrade to doxygen 1.8.13

## Removed
- Disable log4tango benchmark tests.

### Fixed
- Fix memory leak in zmq_event_subscription_change() ([#457][i-457], [!460][mr-460])
- Fix bug impacting device servers using dynamic attributes ([#458][i-458], [!459][mr-459])
- Fix reconnection issues when a client subscribes to several events ([#455][i-455])
- Fix some event compatibility issues with device server <= Tango 8 ([#456][i-456])
- Fix event field in EventData structure passed to user's callback (broken in Tango 9.3.0 and 9.3.1)
- Fix attribute name in EventData structure passed to user's callback for Attribute Config events
- Fix a bug occurring when an event is pushed at the same time as an event (re)subscription occurs
([#484][i-484], [!485][mr-485])
- Fix event name (EventData.event) passed to client's Callback after Tango 9 attribute reconnection ([!486][mr-486])
- Fix some doxygen warnings

## [9.3.1] - 2018-05-23 [unstable]
### Changed
- Add -pedantic for GCC/Clang and debug mode ([!441][mr-441]). **This change can generate
  errors with very old compilers (e.g. gcc 3.4.6) when compiling the lib debug
  version.** Please remove manually -pedantic flag in configure/CMakeLists.txt if
  you need to compile the debug version on a very old compiler.
- Generate static library ([#17][i-17])

### Fixed
- DServerSignal: Include errno header instead of trying to manually get the declaration right ([!441][mr-441])
- Remove GNU_CXX definition for GCC/Clang ([!441][mr-441])
- filedatabase.cpp: Remove stray semicolon ([!441][mr-441])
- FileDatabase: Use correct format specifier for unsigned arguments to sscanf ([!441][mr-441])
- EventConsumer::connect_event: Fix misleading indentation ([!441][mr-441])
- PollThread: Fix multiline macro definitions ([!451][mr-451])
- Fix compilation definitions ([!437][mr-437])
- Replace asserts in EventConsumer::initialize_received_from_admin with exceptions ([#453][i-453])

## [9.3.0] - 2018-05-04 [unstable]
### Fixed
- Fix FQDN events ([!423][mr-423], [#315][i-315])
   Limitations:
     - in case DeviceProxy is used for event subscription, client will get incorrect value
	 in the event callback if short domain name is used in env.TANGO_HOST
	 e.g. hzgc103 -> hzgc103.desy.de
- Fix memory leak when pipe event is sent and there is no client ([#447][i-447],[!448][mr-448])
- Set the ZMQ Receive Buffer High Water Mark only if it changes ([#444][i-444],[!445][mr-445])
   to reduce the impact of a bug in ZMQ 4.2.0 and ZMQ 4.2.1 which was fixed in
   commit [zeromq/libzmq@edc770d][libzmq-c-edc770d].

## [9.2.8] - 2018-03-29 [unstable]
### Changed
- Cmake release targets ([!438][mr-438])
- **Install include files in <install_prefix>/include/tango directory instead of
   <install_prefix>/include**.

### Fixed
- Fix appveyor build

## [9.2.7] - 2018-02-27 [unstable]
### Added
- **Use CMake**
- Set CMake minimum requirement to CMake 2.8.12 ([!422][mr-422])
- Add DevVarBooleanArray operators ([!347][mr-347])
- Add support for NaN and inf in device properties ([#360][i-360], [!362][mr-362],[!371][mr-371])
- Refactor DeviceAttribute code ([!394][mr-394]). DeviceAttribute::init_common_class_members() method is now private
- Forwarded dynamic attributes ([!342][mr-342])
- Build on Windows using CMake and setup appveyor CI ([!413][mr-413])
- Enable Travis CI ([!375][mr-375], [!417][mr-417], [!418][mr-418], [!419][mr-419])
- tango_config.h: Add clang detection ([!442][mr-442])
- Doxygen: Add searchbar ([!432][mr-432])

### Changed
- DeviceAttribute::get_type() no longer throws an API_EmptyDeviceAttribute exception when the DeviceAttribute is empty.
  It returns DATA_TYPE_UNKNOWN instead ([!393][mr-393], [#509][i-509]).

  **Warning! This change could lead to Segmentation fault** in code doing things like:
```
    DeviceAttribute da;
    da = device->read_attribute("AnAttributeWithINVALIDQualityFactor");
    cout << "Type = " << Tango::CmdArgTypeName[da.get_type()] << endl;
```
  Please refer to [#510][i-510] for more details.
- Improve ZmqEventSubscriptionChange command argin and argout descriptions ([355da96][c-355da96])
- Generate files from IDL (**add a dependency to tango-idl repository. omniidl is required to be able to generate some
  files from  CORBA IDL**)
- Get exception message from event callback ([!431][mr-431])
- Add a map in MultiAttribute object to improve performances ([#424][i-424], [!430][mr-430])
- A small Doxygen improvement - take version from cmake ([!436][mr-436])
- Reduce event subscription sleeps on linux ([!415][mr-415]).

**Warning**: There is no guarantee that after a successful call to subscribe_event() that a subscriber will receive all
the events it is interested in. In some situations, some important events might be missed if they occur between the
subscribe_event() call and the ZMQ subscription reception on the ZMQ publisher side, potentially leading to situations
where a client application might show out of date/incorrect values.
- Tango 9 LTS fix[#395][i-395] (Inserting const C string in DeviceData) ([!396][mr-396])

### Removed
- Remove zmq.hpp ([#266][i-266], [!421][mr-421]). **Add a dependency to https://github.com/zeromq/cppzmq**

### Fixed
- Add missing event implementation with \_TG_WINDOWS\_ ifdef ([#349][i-349])
- Fix [TangoTickets#3][tt-i3] (Events received with UNKNOWN data format and without values when subscribing from
  a Tango 9 client to a Tango 7 server DevShort attribute)
- Fix bug related to heartbeat endpoints which could eventually occur when the device server runs on a host with
  multiple network interfaces ([36aca17][c-36aca17])
- Fix bug after admin device RestartServer command ([!358][mr-358])
- Fix bug in Attribute::get_att_device_class() ([4a00c8e][c-4a00c8e])
- Fix RcsId warnings ([!374][mr-374])
- Fix warnings reported by gcc 6.3 related to misleading indentations ([!376][mr-376])
- Fix "unused parameter" warnings ([!379][mr-379])
- Fix shift-negative-value compilation warnings reported by g++ 6.3 ([!383][mr-383])
- Fix DeviceAttribute constructor for short and enum data types ([#392][i-392], [!393][mr-393])
- Fix bug leading occasionally to segmentation faults of Tango 9 device servers ([!398][mr-398])
- Fix bug in event name when pushing an error event to IDL5 clients ([!400][mr-400])
- Attribute: Don't treat an integer as an char* ([!403][mr-403], [!406][mr-406])
- pointer-cast-size-mismatch attrgetsetprop.cpp:200 ([#339][i-339], [!406][mr-406])
- Fix build errors when using C++17 ([!405][mr-405], [!425][mr-425])
- Fix issue with polling not starting automatically on dynamic attributes ([!427][mr-427])
- Add noreturn to remove some compilation warnings ([!435][mr-435])
- CPU load when device has large number of attributes ([!404][mr-404])
- push event performance issue ([#244][i-244])

[9.3.5]: https://gitlab.com/tango-controls/cppTango/-/compare?from=9.3.4&to=9.3.5
[9.3.4]: https://gitlab.com/tango-controls/cppTango/-/compare?from=9.3.3&to=9.3.4
[9.3.3]: https://gitlab.com/tango-controls/cppTango/-/compare?from=9.3.2&to=9.3.3
[9.3.2]: https://gitlab.com/tango-controls/cppTango/-/compare?from=9.3.1&to=9.3.2
[9.3.1]: https://gitlab.com/tango-controls/cppTango/-/compare?from=9.3.0&to=9.3.1
[9.3.0]: https://gitlab.com/tango-controls/cppTango/-/compare?from=test-auto-deploy&to=9.3.0
[9.2.8]: https://gitlab.com/tango-controls/cppTango/-/compare?from=9.2.7&to=test-auto-deploy
[9.2.7]: https://gitlab.com/tango-controls/cppTango/-/compare?from=cppapi_Release_9_2_5&to=9.2.7

[c-4a00c8e]: https://gitlab.com/tango-controls/cppTango/-/commit/4a00c8e4203668c247a2ada6e309470cd53744d3
[c-36aca17]: https://gitlab.com/tango-controls/cppTango/-/commit/36aca17d43a97ebe09fdc825941cb39f6c48289e
[c-355da96]: https://gitlab.com/tango-controls/cppTango/-/commit/355da96ac7d82d4a7727f68f3b093c98f087fd98
[c-5c74e8d]: https://gitlab.com/tango-controls/cppTango/-/commit/5c74e8d33ce85036c9348a09b894e05ac3e8cdf9

[libzmq-c-edc770d]: https://github.com/zeromq/libzmq/commit/edc770d680b2be5a633526bd2c724d66406c8360
[libzmq-c-edb4ca1]: https://github.com/zeromq/libzmq/commit/edb4ca1

[tt-i3]: https://gitlab.com/tango-controls/TangoTickets/-/issues/3

[i-17]: https://gitlab.com/tango-controls/cppTango/-/issues/17
[i-244]: https://gitlab.com/tango-controls/cppTango/-/issues/244
[i-266]: https://gitlab.com/tango-controls/cppTango/-/issues/266
[i-273]: https://gitlab.com/tango-controls/cppTango/-/issues/273
[i-275]: https://gitlab.com/tango-controls/cppTango/-/issues/275
[i-315]: https://gitlab.com/tango-controls/cppTango/-/issues/315
[i-330]: https://gitlab.com/tango-controls/cppTango/-/issues/330
[i-339]: https://gitlab.com/tango-controls/cppTango/-/issues/339
[mr-342]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/342
[mr-347]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/347
[i-349]: https://gitlab.com/tango-controls/cppTango/-/issues/349
[mr-358]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/358
[i-359]: https://gitlab.com/tango-controls/cppTango/-/issues/359
[i-360]: https://gitlab.com/tango-controls/cppTango/-/issues/360
[i-369]: https://gitlab.com/tango-controls/cppTango/-/issues/369
[mr-362]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/362
[mr-371]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/371
[mr-374]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/374
[mr-375]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/375
[mr-376]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/376
[mr-379]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/379
[mr-383]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/383
[i-384]: https://gitlab.com/tango-controls/cppTango/-/issues/384
[i-392]: https://gitlab.com/tango-controls/cppTango/-/issues/392
[mr-393]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/393
[mr-394]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/394
[i-395]: https://gitlab.com/tango-controls/cppTango/-/issues/395
[mr-396]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/396
[mr-398]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/398
[mr-400]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/400
[mr-403]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/403
[mr-404]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/404
[mr-405]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/405
[mr-406]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/406
[mr-413]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/413
[mr-415]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/415
[mr-417]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/417
[mr-418]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/418
[mr-419]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/419
[mr-421]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/421
[mr-422]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/422
[mr-423]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/423
[i-424]: https://gitlab.com/tango-controls/cppTango/-/issues/424
[mr-425]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/425
[mr-427]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/427
[i-428]: https://gitlab.com/tango-controls/cppTango/-/issues/428
[mr-430]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/430
[mr-431]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/431
[mr-432]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/432
[mr-435]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/435
[mr-436]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/436
[mr-437]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/437
[mr-438]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/438
[i-439]: https://gitlab.com/tango-controls/cppTango/-/issues/439
[mr-441]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/441
[mr-442]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/442
[i-444]: https://gitlab.com/tango-controls/cppTango/-/issues/444
[mr-445]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/445
[i-447]: https://gitlab.com/tango-controls/cppTango/-/issues/447
[mr-448]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/448
[mr-451]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/451
[i-453]: https://gitlab.com/tango-controls/cppTango/-/issues/453
[i-455]: https://gitlab.com/tango-controls/cppTango/-/issues/455
[i-456]: https://gitlab.com/tango-controls/cppTango/-/issues/456
[i-457]: https://gitlab.com/tango-controls/cppTango/-/issues/457
[i-458]: https://gitlab.com/tango-controls/cppTango/-/issues/458
[mr-459]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/459
[mr-460]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/460
[mr-474]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/474
[mr-476]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/476
[mr-478]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/478
[i-484]: https://gitlab.com/tango-controls/cppTango/-/issues/484
[mr-485]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/485
[mr-486]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/486
[mr-488]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/488
[i-492]: https://gitlab.com/tango-controls/cppTango/-/issues/492
[mr-493]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/493
[i-499]: https://gitlab.com/tango-controls/cppTango/-/issues/499
[mr-502]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/502
[mr-503]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/503
[mr-508]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/508
[i-509]: https://gitlab.com/tango-controls/cppTango/-/issues/509
[i-510]: https://gitlab.com/tango-controls/cppTango/-/issues/510
[i-511]: https://gitlab.com/tango-controls/cppTango/-/issues/511
[i-514]: https://gitlab.com/tango-controls/cppTango/-/issues/514
[mr-516]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/516
[mr-517]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/517
[mr-518]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/518
[mr-520]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/520
[mr-521]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/521
[mr-522]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/522
[mr-523]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/523
[mr-529]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/529
[mr-530]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/530
[mr-531]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/531
[i-532]: https://gitlab.com/tango-controls/cppTango/-/issues/532
[mr-533]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/533
[i-535]: https://gitlab.com/tango-controls/cppTango/-/issues/535
[mr-537]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/537
[mr-539]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/539
[i-541]: https://gitlab.com/tango-controls/cppTango/-/issues/541
[mr-542]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/542
[mr-544]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/544
[i-546]: https://gitlab.com/tango-controls/cppTango/-/issues/546
[mr-549]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/549
[mr-561]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/561
[mr-597]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/597
[mr-621]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/621
[mr-629]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/629
[mr-639]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/639
[mr-647]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/647
[mr-658]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/658
[mr-664]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/664
[mr-665]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/665
[mr-674]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/674
[mr-677]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/677
[mr-681]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/681
[mr-683]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/683
[i-686]: https://gitlab.com/tango-controls/cppTango/-/issues/686
[mr-689]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/689
[mr-690]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/690
[mr-699]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/699
[i-496]: https://gitlab.com/tango-controls/cppTango/-/issues/496
[mr-596]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/596
[i-740]: https://gitlab.com/tango-controls/cppTango/-/issues/740
[mr-702]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/702
[mr-709]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/709
[mr-716]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/716
[mr-738]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/738
[mr-743]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/743
[mr-751]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/751
[mr-763]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/763
[mr-780]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/780
[i-675]: https://gitlab.com/tango-controls/cppTango/-/issues/675
[mr-778]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/778
[i-816]: https://gitlab.com/tango-controls/cppTango/-/issues/816
[i-675]: https://gitlab.com/tango-controls/cppTango/-/issues/675
[i-823]: https://gitlab.com/tango-controls/cppTango/-/issues/823
[i-842]: https://gitlab.com/tango-controls/cppTango/-/issues/842
[i-767]: https://gitlab.com/tango-controls/cppTango/-/issues/767
[i-409]: https://gitlab.com/tango-controls/cppTango/-/issues/409
[i-858]: https://gitlab.com/tango-controls/cppTango/-/issues/858
[i-691]: https://gitlab.com/tango-controls/cppTango/-/issues/691
[i-880]: https://gitlab.com/tango-controls/cppTango/-/issues/880
[mr-818]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/818
[mr-780]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/780
[mr-778]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/778
[mr-824]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/824
[mr-828]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/828
[mr-824]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/824
[mr-827]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/827
[mr-842]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/842
[mr-841]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/841
[mr-863]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/863
[mr-873]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/873
[mr-866]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/866
[mr-893]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/893
[mr-825]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/825
[mr-900]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/900
[mr-909]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/909
[mr-905]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/905
[mr-852]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/852
[mr-896]: https://gitlab.com/tango-controls/cppTango/-/merge_requests/896
[i-550]: https://gitlab.com/tango-controls/cppTango/-/issues/550
[i-545]: https://gitlab.com/tango-controls/cppTango/-/issues/545
[i-839]: https://gitlab.com/tango-controls/cppTango/-/issues/839
[i-843]: https://gitlab.com/tango-controls/cppTango/-/issues/843
