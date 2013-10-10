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

#ifndef DESURA_STEAMUSER_H
#define DESURA_STEAMUSER_H
#ifdef _WIN32
#pragma once
#endif

#include "wx_controls/gcControls.h"
#include "wx/wx.h"



class SteamUserDialog : public gcDialog 
{
public:
	SteamUserDialog(wxWindow* parent);
	~SteamUserDialog();

	const char* getSteamUser(){return m_szSteamUser.c_str();}

	int ShowModal();

protected:
	wxStaticText* m_labInfo;
	gcChoice* m_cbSteamUser;
	gcButton* m_butOk;
	
	void onButClick( wxCommandEvent& event );
	void saveSteamUser();

private:
	gcString m_szSteamUser;

	DECLARE_EVENT_TABLE();
};

#endif //DESURA_STEAMUSER_H
