#ifndef SeqVecTestSuite_h
#define SeqVecTestSuite_h

#include "cxx_common.h"

#undef SUITE_NAME
#define SUITE_NAME SeqVecTestSuite

class SeqVecTestSuite: public CxxTest::TestSuite
{
protected:
	DeviceProxy *device1;

public:
	SUITE_NAME()
	{

//
// Arguments check -------------------------------------------------
//

		string device1_name;

		device1_name = CxxTest::TangoPrinter::get_param("device1");

		CxxTest::TangoPrinter::validate_args();


//
// Initialization --------------------------------------------------
//

		try
		{
			device1 = new DeviceProxy(device1_name);
			device1->ping();
		}
		catch (CORBA::Exception &e)
		{
			Except::print_exception(e);
			exit(-1);
		}

	}

	virtual ~SUITE_NAME()
	{
		delete device1;
	}

	static SUITE_NAME *createSuite()
	{
		return new SUITE_NAME();
	}

	static void destroySuite(SUITE_NAME *suite)
	{
		delete suite;
	}

//
// Tests -------------------------------------------------------
//

// Test IOSeqVecChar

	void test_IOSeqVecChar(void)
	{
		DeviceData din, dout;
		vector<DevUChar> input(3), output(3);
		input[0] = 'a';
		input[1] = 'b';
		input[2] = '1';
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecChar", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecShort

	void test_IOSeqVecShort(void)
	{
		DeviceData din, dout;
		vector<DevShort> input(4), output(4);
		input[0] = 10;
		input[1] = 20;
		input[2] = 30;
		input[3] = 40;
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecShort", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecLong

	void test_IOSeqVecLong(void)
	{
		DeviceData din, dout;
		vector<DevLong> input(1), output(1);
		input[0] = -656;
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecLong", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecFloat

	void test_IOSeqVecFloat(void)
	{
		DeviceData din, dout;
		vector<DevFloat> input(3), output(3);
		input[0] = 1.11f;
		input[1] = 2.22f;
		input[2] = 3.33f;
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecFloat", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecDouble

	void test_IOSeqVecDouble(void)
	{
		DeviceData din, dout;
		vector<DevDouble> input(2), output(2);
		input[0] = 0.246;
		input[1] = 6.66;
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecDouble", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecUShort

	void test_IOSeqVecUShort(void)
	{
		DeviceData din, dout;
		vector<DevUShort> input(6), output(6);
		input[0] = 111;
		input[1] = 222;
		input[2] = 333;
		input[3] = 444;
		input[4] = 555;
		input[5] = 666;
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecUShort", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecULong

	void test_IOSeqVecULong(void)
	{
		DeviceData din, dout;
		vector<DevULong> input(2), output(2);
		input[0] = 345;
		input[1] = 678;
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecULong", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}

// Test IOSeqVecString

	void test_IOSeqVecString(void)
	{
		DeviceData din, dout;
		vector<string> input(3), output(3);
		input[0] = "a";
		input[1] = "b";
		input[2] = "cccc";
		din << input;
		try
		{
			dout = device1->command_inout("IOSeqVecString", din);
		}
		catch(DevFailed &e)
		{
			Except::print_exception(e);
		}
		dout >> output;
		TS_ASSERT_EQUALS(input, output);
	}
};
#undef cout
#endif // SeqVecTestSuite_h
