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

#include "dlgOptions.h"
#include <wx/config.h>

dlgOptions::dlgOptions( wxWindow* parent, obdbase* device, struct obdOptions* opts)
:
dlgBaseOptions( parent )
{
	wxArrayString devices;

	obd = device;
	options = opts;

#if defined (WIN32)
	devices.Add(_T("COM1"));
	devices.Add(_T("COM2"));
	devices.Add(_T("COM3"));
	devices.Add(_T("COM4"));
#else
	devices.Add(_T("/dev/ttyS0"));
	devices.Add(_T("/dev/ttyS1"));
	devices.Add(_T("/dev/ttyS2"));
	devices.Add(_T("/dev/ttyS3"));
	devices.Add(_T("/dev/rfcomm0"));
#endif

	cmb_Port->Append(devices);
	cmb_Port->SetSelection(0);

	if (obd->obd_is_connected()) {
		cmb_Port->Enable(false);
	}

	cmb_Port->SetValue(options->port);
	check_Imperial->SetValue(options->imperial);

	// get other config options
	wxConfigBase *pConfig = wxConfigBase::Get();
    checkStartUp->SetValue(pConfig->Read(_T("/Options/AtStartup"), 0l));
}

void dlgOptions::onOK( wxCommandEvent& WXUNUSED(event) )
{
	// get the current options on the dialog and update the struct
	options->imperial = check_Imperial->IsChecked();
	options->port = cmb_Port->GetValue();
	obd->obd_use_imperial(options->imperial);

	// wxConfig stuff
    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->Write(_T("/Options/Imperial"), options->imperial);
    pConfig->Write(_T("/Options/Port"), options->port);
    pConfig->Write(_T("/Options/AtStartup"), checkStartUp->IsChecked());

	this->EndModal(wxID_OK);
}
