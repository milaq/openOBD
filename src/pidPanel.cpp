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
    #include <wx/msgdlg.h>
#endif

#include <vector>
#include "pidPanel.h"

pidPanel::pidPanel( wxWindow* parent, obdbase* device, sqlite3* sql  )
:
pidBasePanel( parent )
{
    // get the parameters passed in constructor
    obd = device;
    db = sql;

    // setup the ListCtrl columns
    pidList->InsertColumn(0, _("PID"), wxLIST_FORMAT_LEFT, -1);
	pidList->InsertColumn(1, _("Description"), wxLIST_FORMAT_LEFT, -1);
	pidList->InsertColumn(2, _("Value"), wxLIST_FORMAT_LEFT, -1);
	pidList->InsertColumn(3, _("Units"), wxLIST_FORMAT_LEFT, -1);
}

void pidPanel::onRefreshClick( wxCommandEvent& WXUNUSED(event) )
{
    obdbase::pidInfo result;
    std::vector<int> pids;
    vector<int>::iterator it;
    long itemIndex;
    wxString pidString;
    wxString resultString;
    wxString descString;
    wxString unitString;
    wxString sql(_T("SELECT * FROM pids WHERE pid = ?1"));
	sqlite3_stmt *stmt;
	char buf[100];
	bool imperial;

	if (!obd->obd_is_connected()) {
	    wxString msg(_("You are not connected to an ELM device.\n"
            "Please connect first"));
        wxMessageDialog dialog(NULL, msg, _("Error"), wxOK | wxICON_ERROR);
        dialog.ShowModal();
	} else {
	    obd->obd_pid_supported_pids(pids);
	    pidList->DeleteAllItems();
	    imperial = obd->obd_is_imperial();

	     // prepare the statement for the db
        if (sqlite3_prepare_v2(db, sql.mb_str(), -1, &stmt, NULL) == SQLITE_OK)
        {
            // parse each pid in turn
            for (it = pids.begin(); it < pids.end(); it++) {
                // get the value of the pid
                if (obd->obd_pid_value(*it, &result)) {

                    pidString.Printf(_T("%#.4x"), *it);
                    itemIndex = pidList->InsertItem(0, pidString);

                    // get the results from the db
                    // bind the pid to the statement
                    strcpy( buf, (const char*)pidString.mb_str(wxConvUTF8) );
                    int sqlResult = sqlite3_bind_text(stmt, 1, buf, -1, SQLITE_STATIC);

                    // the statement should only have one step
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // insert the code and description into the list control
                        descString = wxString::FromUTF8((const char *)sqlite3_column_text(stmt, 1));
                        pidList->SetItem(itemIndex, 1, descString);

                        if (imperial) {
                            unitString = wxString::FromUTF8((const char *)sqlite3_column_text(stmt, 3));
                        } else {
                            unitString = wxString::FromUTF8((const char *)sqlite3_column_text(stmt, 2));
                        }
                        pidList->SetItem(itemIndex, 3, unitString);

                    }

                    // update the results column
                    switch (result.pid_flag) {
                        case obdbase::PID_FLAG_SINGLE:
                            resultString.Printf(_T("%f\n"), result.resultMain);
                            break;
                        case obdbase::PID_FLAG_DOUBLE:
                            resultString.Printf(_T("%f / %f\n"), result.resultMain, result.resultSecondary);
                            break;
                        case obdbase::PID_FLAG_STRING:
                            resultString = result.resultString;
                            break;
                    }
                    pidList->SetItem(itemIndex, 2, resultString);

                    // reset the sql
                    sqlite3_reset(stmt);

                    // new result added so update the GUI
                    this->Update();
                }
            }

            // don't need the stmt any more
            sqlite3_finalize(stmt);
        }
	}
}   // onRefreshClick()

void pidPanel::updateDevice(obdbase* device)
{
    obd = device;
}
