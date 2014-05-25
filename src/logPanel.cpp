/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * openobd
 * Copyright (C) Simon Booth 2010 <simesb@users.sourceforge.net>
 *
 * openobd is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * openobd is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logPanel.h"
#include <time.h>

logPanel::logPanel( wxWindow* parent )
:
logBasePanel( parent )
{

}

void logPanel::appendLog(wxString& logText, logType type)
{
	time_t rawtime;
	wxString timestamp;
	wxColour colour;

	time ( &rawtime );
	wxDateTime now(rawtime);

	switch (type) {
		case LOG_IN:
			colour.Set(_T("BLUE"));
			break;
		case LOG_OUT:
			colour.Set(_T("GREEN"));
			break;
		case LOG_ERROR:
			colour.Set(_T("RED"));
			break;
		case LOG_OTHER:
			colour.Set(_T("BLACK"));
			break;
	}
	timestamp = now.Format(_T("[%c] "));
	m_richText1->BeginTextColour(colour);
	m_richText1->AppendText(timestamp + logText);
}

void logPanel::SaveFile(wxString& path)
{
	m_richText1->SaveFile(path, wxRICHTEXT_TYPE_TEXT);
}

