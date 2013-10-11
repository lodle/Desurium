

#include "Common.h"
#include <gtest/gtest.h>


namespace UnitTest
{

	static bool g_bHitCallback = false;
	static int g_nHitVal = -1;

	static void CallbackVoid()
	{
		g_bHitCallback = true;
	}

	static void Callback(int &val)
	{
		g_bHitCallback = true;
		g_nHitVal = val;
	}

	class StubCallback
	{
	public:
		StubCallback()
			: m_bHitCallback(false)
			, m_nExtra(-1)
			, m_nHitVal(-1)
		{	
		}

		void CallbackVoid()
		{
			m_bHitCallback = true;
		}

		void CallbackVoidExtra(int nExtra)
		{
			m_bHitCallback = true;
			m_nExtra = nExtra;
		}


		void Callback(int &val)
		{
			m_nHitVal = val;
			m_bHitCallback = true;
		}

		void CallbackExtra(int nExtra, int &val)
		{
			m_nHitVal = val;
			m_bHitCallback = true;
			m_nExtra = nExtra;
		}

		bool m_bHitCallback;
		int m_nHitVal;
		int m_nExtra;
	};


	TEST(Event, StaticFunction_Void)
	{
		g_bHitCallback = false;

		EventV ev;
		ev += delegate(&CallbackVoid);

		ev();
		ASSERT_TRUE(g_bHitCallback);

		g_bHitCallback = false;
		ev -= delegate(&CallbackVoid);

		ev();
		ASSERT_FALSE(g_bHitCallback);
	}

	TEST(Event, StaticFunction_Void_Obj)
	{
		StubCallback cb;

		EventV ev;
		ev += delegate(&cb, &StubCallback::CallbackVoid);

		ev();
		ASSERT_TRUE(cb.m_bHitCallback);

		cb.m_bHitCallback = false;
		ev -= delegate(&cb, &StubCallback::CallbackVoid);

		ev();
		ASSERT_FALSE(cb.m_bHitCallback);
	}


	TEST(Event, StaticFunction_Void_ObjExtra)
	{
		StubCallback cb;

		EventV ev;
		ev += extraDelegate(&cb, &StubCallback::CallbackVoidExtra, 24);

		ev();
		ASSERT_TRUE(cb.m_bHitCallback);
		ASSERT_EQ(24, cb.m_nExtra);

		cb.m_bHitCallback = false;
		ev -= extraDelegate(&cb, &StubCallback::CallbackVoidExtra, 24);

		ev();
		ASSERT_FALSE(cb.m_bHitCallback);
	}

	TEST(Event, StaticFunction_Void_Obj_Proxy)
	{
		StubCallback cb;

		EventV ev;
		ev += delegate(&cb, &StubCallback::CallbackVoid);

		EventV ev2;
		ev2 += delegate(&ev);

		ev2();
		ASSERT_TRUE(cb.m_bHitCallback);

		cb.m_bHitCallback = false;
		ev -= delegate(&cb, &StubCallback::CallbackVoid);

		ev2();
		ASSERT_FALSE(cb.m_bHitCallback);
	}




	TEST(Event, StaticFunction)
	{
		int nVal = 89;

		g_bHitCallback = false;

		Event<int> ev;
		ev += delegate(&Callback);

		ev(nVal);
		ASSERT_EQ(89, g_nHitVal);
		ASSERT_TRUE(g_bHitCallback);

		g_nHitVal = -1;
		g_bHitCallback = false;
		ev -= delegate(&Callback);

		ev(nVal);
		ASSERT_FALSE(g_bHitCallback);
	}

	TEST(Event, StaticFunction_Obj)
	{
		int nVal = 89;
		StubCallback cb;

		Event<int> ev;
		ev += delegate(&cb, &StubCallback::Callback);

		ev(nVal);
		ASSERT_EQ(89, cb.m_nHitVal);
		ASSERT_TRUE(cb.m_bHitCallback);

		cb.m_bHitCallback = false;
		ev -= delegate(&cb, &StubCallback::Callback);

		ev(nVal);
		ASSERT_FALSE(cb.m_bHitCallback);
	}


	TEST(Event, StaticFunction_ObjExtra)
	{
		int nVal = 89;
		StubCallback cb;

		Event<int> ev;
		ev += extraDelegate(&cb, &StubCallback::CallbackExtra, 24);

		ev(nVal);
		ASSERT_TRUE(cb.m_bHitCallback);
		ASSERT_EQ(24, cb.m_nExtra);
		ASSERT_EQ(89, cb.m_nHitVal);

		cb.m_bHitCallback = false;
		ev -= extraDelegate(&cb, &StubCallback::CallbackExtra, 24);

		ev(nVal);
		ASSERT_FALSE(cb.m_bHitCallback);
	}

	TEST(Event, StaticFunction_Obj_EventProxy)
	{
		int nVal = 89;
		StubCallback cb;

		Event<int> ev;
		ev += delegate(&cb, &StubCallback::Callback);

		Event<int> ev2;
		ev2 += delegate(&ev);

		ev2(nVal);
		ASSERT_EQ(89, cb.m_nHitVal);
		ASSERT_TRUE(cb.m_bHitCallback);

		cb.m_bHitCallback = false;
		ev -= delegate(&cb, &StubCallback::Callback);

		ev2(nVal);
		ASSERT_FALSE(cb.m_bHitCallback);
	}




	class SubRemoveDuringCallback
	{
	public:
		SubRemoveDuringCallback(Event<int>& ev)
			: m_Ev(ev)
			, m_nHitCount(0)
		{
			m_Ev += delegate(this, &SubRemoveDuringCallback::Callback);
		}

		void Callback(int &val)
		{
			m_nHitCount++;
			m_Ev -= delegate(this, &SubRemoveDuringCallback::Callback);
		}

		int m_nHitCount;
		Event<int>& m_Ev;
	};


	TEST(Event, StaticFunction_Obj_DeRegDuringCallback)
	{
		int nVal = 89;

		Event<int> ev;
		SubRemoveDuringCallback cb(ev);

		ev(nVal);
		ASSERT_EQ(1, cb.m_nHitCount);

		ev(nVal);
		ASSERT_EQ(1, cb.m_nHitCount);
	}


	class SubRegDuringCallback
	{
	public:
		SubRegDuringCallback(Event<int>& ev)
			: m_Ev(ev)
			, m_nHitCount(0)
			, m_nHitCount2(0)
		{
			m_Ev += delegate(this, &SubRegDuringCallback::Callback);
		}

		void Callback(int &val)
		{
			m_nHitCount++;
			m_Ev += delegate(this, &SubRegDuringCallback::Callback2);
		}

		void Callback2(int &val)
		{
			m_nHitCount2++;
		}

		int m_nHitCount;
		int m_nHitCount2;
		Event<int>& m_Ev;
	};

	TEST(Event, StaticFunction_Obj_RegDuringCallback)
	{
		int nVal = 89;

		Event<int> ev;
		SubRegDuringCallback cb(ev);

		ev(nVal);
		ASSERT_EQ(1, cb.m_nHitCount);
		ASSERT_EQ(1, cb.m_nHitCount2);

		ev(nVal);
		ASSERT_EQ(2, cb.m_nHitCount);
		ASSERT_EQ(2, cb.m_nHitCount2);
	}



}