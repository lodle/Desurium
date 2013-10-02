/*
Desura is the leading indie game distribution platform
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "Common.h"
#include "IPCToolMain.h"
#include "IPCManager.h"

#ifndef DESURA_CLIENT
#include "ToolInstallThread.h"
#else
#include "ToolInfo.h"
#endif

IPCToolMain* g_pToolMain = NULL;
REG_IPC_CLASS( IPCToolMain );


#ifndef DESURA_CLIENT

#include "log.h"
#include "LogCallback.h"
extern LogCallback* g_pLogCallback;

bool g_bLogEnabled = true;


void IPCTool_Msg(const char* msg, Color *col = NULL)
{
	if (!g_pToolMain || !g_bLogEnabled)
		return;

	if (!msg)
		return;

	g_pToolMain->message(msg);
}

void IPCTool_Msg_W(const wchar_t* msg, Color *col = NULL)
{
	if (!g_pToolMain || !g_bLogEnabled)
		return;

	if (!msg)
		return;

	g_pToolMain->message(gcString(msg).c_str());
}

void IPCTool_Warn(const char* msg)
{
	if (!g_pToolMain || !g_bLogEnabled)
		return;

	if (!msg)
		return;

	g_pToolMain->warning(msg);
}

void IPCTool_Warn_W(const wchar_t* msg)
{
	if (!g_pToolMain || !g_bLogEnabled)
		return;

	if (!msg)
		return;

	g_pToolMain->warning(gcString(msg).c_str());
}

void IPCTool_Debug(const char* msg)
{
	if (!g_pToolMain || !g_bLogEnabled)
		return;

#ifdef DEBUG
	if (!msg)
		return;

	g_pToolMain->debug(msg);
#endif
}

void IPCTool_Debug_W(const wchar_t* msg)
{
	if (!g_pToolMain || !g_bLogEnabled)
		return;

#ifdef DEBUG
	if (!msg)
		return;

	g_pToolMain->debug(gcString(msg).c_str());
#endif
}

#endif


IPCToolMain::IPCToolMain(IPC::IPCManager* mang, uint32 id, DesuraId itemId) : IPC::IPCClass(mang, id, itemId)
{
	registerFunctions();
	g_pToolMain = this;

#ifndef DESURA_CLIENT
	g_pLogCallback->RegMsg(&IPCTool_Msg);
	g_pLogCallback->RegMsg(&IPCTool_Msg_W);
	g_pLogCallback->RegWarn(&IPCTool_Warn);
	g_pLogCallback->RegWarn(&IPCTool_Warn_W);
	g_pLogCallback->RegDebug(&IPCTool_Debug);
	g_pLogCallback->RegDebug(&IPCTool_Debug_W);

	m_pThread = NULL;
#endif
}

IPCToolMain::~IPCToolMain()
{
#ifndef DESURA_CLIENT
	safe_delete(m_pThread);
#endif
}

void IPCToolMain::registerFunctions()
{
#ifndef DESURA_CLIENT
	REG_FUNCTION_VOID(IPCToolMain, setCrashSettings);
	REG_FUNCTION(IPCToolMain, installTool);

	LINK_EVENT(onErrorEvent, gcException);
	LINK_EVENT(onCompleteEvent, int32);
#else
	REG_EVENT(onErrorEvent);
	REG_EVENT(onCompleteEvent);

	REG_FUNCTION_VOID(IPCToolMain, warning);
	REG_FUNCTION_VOID(IPCToolMain, message);
	REG_FUNCTION_VOID(IPCToolMain, debug);
#endif
}


#ifdef DESURA_CLIENT

void IPCToolMain::warning(const char* msg)
{
	Warning(gcString("ToolHelper: {0}", msg));
}

void IPCToolMain::message(const char* msg)
{
	Msg(gcString("ToolHelper: {0}", msg));
}

void IPCToolMain::debug(const char* msg)
{
	Debug(gcString("ToolHelper: {0}", msg));
}	

gcException IPCToolMain::installTool(UserCore::ToolInfo* info)
{
	return installTool(info->getExe(), info->getArgs());
}

gcException IPCToolMain::installTool(const char* exe, const char* args)
{
	try
	{
		return IPC::functionCall<gcException>(this, "installTool", exe, args);
	}
	catch (gcException &e)
	{
		return e;
	}
}

void IPCToolMain::setCrashSettings(const char* user, bool upload)
{
	IPC::functionCallAsync(this, "setCrashSettings", user, upload);
}

#else

void IPCToolMain::warning(const char* msg)
{
	IPC::functionCallAsync(this, "warning", msg);
}

void IPCToolMain::message(const char* msg)
{
	IPC::functionCallAsync(this, "message", msg);
}

void IPCToolMain::debug(const char* msg)
{
	IPC::functionCallAsync(this, "debug", msg);
}

gcException IPCToolMain::installTool(const char* exe, const char* args)
{
	if (!m_pThread)
	{
		m_pThread = new ToolInstallThread();
		m_pThread->start();

		m_pThread->onCompleteEvent += delegate(&onCompleteEvent);
		m_pThread->onErrorEvent += delegate(&onErrorEvent);
	}

	return m_pThread->installTool(exe, args);
}

//void SetCrashSettings(const wchar_t* user, bool upload);
void IPCToolMain::setCrashSettings(const char* user, bool upload)
{
	//gcWString wUser(user);
	//SetCrashSettings(wUser.c_str(), upload);
}

#endif