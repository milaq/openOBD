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

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
	#include <wx/aui/auibook.h>
	#include <wx/msgdlg.h>
	#include <wx/filedlg.h>
#endif

#include <wx/aboutdlg.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <vector>
#include "obdFrame.h"
#include "header.h"
#include "obdbase.h"
#include "elm327.h"
#include "logPanel.h"
#include "milPanel.h"
#include "dlgOptions.h"

obdFrame::obdFrame( wxWindow* parent )
:
obdBaseFrame( parent )
{
	int rc;
	wxString logText(_("New Session started\n"));
	bool connectAtStartup;

	obd = new obdbase();
	log = new logPanel (m_auinotebook1);
	obd->obd_set_logger(log);
	log->appendLog(logText, logPanel::LOG_OTHER);
	m_auinotebook1->AddPage( log, _("Log"), false, wxNullBitmap );
	this->updateMenus(false);

    // setup the sqlite3 database
    // check for a valid db in the userdatadir
    wxFileName dbName( wxStandardPaths::Get().GetUserDataDir(), _T("openobd"), _T("db"));
    // if not
    if (!dbName.FileExists()) {
        // check for a valid db in likely places
#if defined (WIN32)
        wxFileName dbSrcName( wxStandardPaths::Get().GetDataDir() + _T("/data/"), _T("openobd"), _T("db"));
#else
        wxFileName dbSrcName( wxStandardPaths::Get().GetDataDir(), _T("openobd"), _T("db"));
#endif
        if (!dbSrcName.FileExists()) {
            // not found so display error
            wxString msg(_("Cannot find the openOBD database: some functionality may be missing"));
            wxMessageDialog dialog(NULL, msg, _("Error"), wxOK | wxICON_ERROR);
            dialog.ShowModal();
        } else {
            // found so copy to userdatadir
            wxCopyFile(dbSrcName.GetFullPath(), dbName.GetFullPath());
        }
    }

    // open the database
    rc = sqlite3_open_v2(dbName.GetFullPath().mb_str(), &db, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK) {
        wxString msg(_("Cannot open the openOBD database: some functionality may be missing"));
        wxMessageDialog dialog(NULL, msg, _("Error"), wxOK | wxICON_ERROR);
        dialog.ShowModal();
        sqlite3_close(db);
    }

	// setup the options
	wxConfigBase *pConfig = wxConfigBase::Get();
	options.imperial = pConfig->Read(_T("/Options/Imperial"), 0l);
#if defined (WIN32)
	options.port = pConfig->Read(_T("/Options/Port"), _T("COM1"));
#else
	options.port = pConfig->Read(_T("/Options/Port"), _T("/dev/ttyS0"));
#endif

    connectAtStartup = pConfig->Read(_T("/Options/AtStartup"), 0l);
    if (connectAtStartup) {
        this->connectDevice();
    }
}

obdFrame::~obdFrame()
{
   sqlite3_close(db);
}

void obdFrame::onMenuExport( wxCommandEvent& WXUNUSED(event) )
{
	wxString filter = wxRichTextBuffer::GetExtWildcard(false, true);
    wxString path;
    wxString filename;

	// call up a dialog for filename
    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_SAVE | wxOVERWRITE_PROMPT);

	// if positive return, save file.
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			log->SaveFile(path);
        }
    }
}

void obdFrame::onMenuQuit( wxCommandEvent& WXUNUSED(event) )
{
	this->Close();
}

void obdFrame::onMenuViewLog( wxCommandEvent& WXUNUSED(event) )
{
	int index;

	index = m_auinotebook1->GetPageIndex(log);
	if (index == wxNOT_FOUND) {
		m_auinotebook1->AddPage( log, _("Log"), false, wxNullBitmap );
		index = m_auinotebook1->GetPageIndex(log);
		m_auinotebook1->SetSelection(index);
		menuViewLog->Check(true);
	} else if (m_auinotebook1->GetPageCount() == 1) {
		wxMessageDialog *dialog = new wxMessageDialog(NULL,
      		_("You may not close the last tab!"), wxT("Error"),
      		wxICON_ERROR);
  		dialog->ShowModal();
		menuViewLog->Check(true);
	} else if (m_auinotebook1->GetSelection() != index) {
		m_auinotebook1->SetSelection(index);
		menuViewLog->Check(true);
	} else {
		m_auinotebook1->RemovePage(index);
		menuViewLog->Check(false);
	}
}

void obdFrame::onMenuViewMIL( wxCommandEvent& WXUNUSED(event) )
{
	int index;

	index = m_auinotebook1->GetPageIndex(mil);

	if (index == wxNOT_FOUND) {
		mil = new milPanel(this, obd, db);
		m_auinotebook1->AddPage(mil, _("MIL Status"), true, wxNullBitmap);
		mil->updateStatus();
		menuViewMIL->Check(true);
	} else if (m_auinotebook1->GetSelection() != index) {
		m_auinotebook1->SetSelection(index);
		menuViewMIL->Check(true);
	} else {
		m_auinotebook1->DeletePage(index);
		menuViewMIL->Check(false);
		if (!obd->obd_is_connected()) {
			menuViewMIL->Enable(false);
		}
	}
}

bool obdFrame::connectDevice()
{
    elm327* elm;
    logPanel::logType type = logPanel::LOG_OUT;
    bool retVal = false;
    wxString logText;

    elm = new elm327(options.port);
    if (elm->obd_is_connected()) {
		obd = elm;
		obd->obd_set_logger (log);
		this->updateMenus(true);
		logText.Printf(_("Connected to device on %s\n"), options.port.c_str());
        log->appendLog(logText, type);

        // restore our imperial preferences
		obd->obd_use_imperial(options.imperial);

		retVal = true;
    } else {
        delete elm;
    }

    return retVal;
}

void obdFrame::onMenuConnect( wxCommandEvent& WXUNUSED(event) )
{
	wxString logText;
	logPanel::logType type = logPanel::LOG_OUT;
	bool connected;

	if (obd->obd_is_connected()) {
		obd->obd_device_disconnect();
		this->updateMenus(false);
		logText.Printf(_("Disconnected from device on %s\n"), options.port.c_str());
		log->appendLog(logText, type);
	} else {
        connected = this->connectDevice();

        if (connected) {
			if (menuViewPIDS->IsChecked()) {
			    pid->updateDevice(obd);
			}

			if (menuViewMIL->IsChecked()) {
			    mil->updateDevice(obd);
			}
		} else {
		    // get rid of the failed connection and update menus
			this->updateMenus(false);

			// write a log message
			logText.Printf(_("Unable to connected to device on %s\n"), options.port.c_str());
			type = logPanel::LOG_ERROR;
			log->appendLog(logText, type);

			// can't connect so display error
            wxString msg;
            msg.Printf(_("Cannot find the inteface device on %s.\n"
                "Please check your settings"), options.port.c_str());
            wxMessageDialog dialog(NULL, msg, _("Error"), wxOK | wxICON_ERROR);
            dialog.ShowModal();
		}
	}
}

void obdFrame::onMenuPrefs( wxCommandEvent& WXUNUSED(event) )
{
	dlgOptions* dlg = new dlgOptions (this, obd, &options);
	dlg->ShowModal();
}

void obdFrame::onMenuViewPIDs( wxCommandEvent& WXUNUSED(event) )
{
    int index;

	index = m_auinotebook1->GetPageIndex(pid);

	if (index == wxNOT_FOUND) {
		pid = new pidPanel(this, obd, db);
		m_auinotebook1->AddPage(pid, _("Live PIDS"), true, wxNullBitmap);
		menuViewPIDS->Check(true);
	} else if (m_auinotebook1->GetSelection() != index) {
		m_auinotebook1->SetSelection(index);
		menuViewPIDS->Check(true);
	} else {
		m_auinotebook1->DeletePage(index);
		menuViewPIDS->Check(false);
		if (!obd->obd_is_connected()) {
		}
	}
}

void obdFrame::onMenuAbout( wxCommandEvent& WXUNUSED(event) )
{
	wxAboutDialogInfo info;
	wxArrayString devs;

	devs.Add(_T("Simon Booth <simesb@users.sourceforge.net>"));
	devs.Add(_("CTB code from https://iftools.com/opensource/ctb.en.php"));

	info.SetName(_("openOBD"));
	info.SetVersion(_T(openobd_version));
	info.SetDescription(_("Opensource OBD-II interface device controller\n" \
				"Released under GPLv3"));
	info.SetDevelopers(devs);
	info.SetCopyright(_("Copyright (C) 2010 Simon Booth <simesb@users.sourceforge.net>"));
	info.SetWebSite(_T("http://openobd.sourceforge.net"));

	wxAboutBox(info);

}

void obdFrame::updateMenus(bool connected)
{
    wxString statusText;

    // Update the menu strings for connect/disconnect
    // TODO: add status bar code here too
    if (connected) {
        menuConnect->SetText(_("&Disconnect from interface"));
        menuConnect->SetHelp(_("Disconnect from OBD-II interface"));
		statusText.Printf(_("Connected: %s (%s)"), obd->obd_identify_device().c_str(),
            obd->obd_get_protocol().c_str());
    } else {
        menuConnect->SetText(_("&Connect to interface"));
		menuConnect->SetHelp(_("Connect to OBD-II interface"));
		statusText = _("Not connected");
    }

    // mil panel
    menuViewMIL->Enable(menuViewMIL->IsChecked() || connected);

    // pid panel
    menuViewPIDS->Enable(menuViewPIDS->IsChecked() || connected);

    m_statusBar1->SetStatusText(statusText, 1);
}
