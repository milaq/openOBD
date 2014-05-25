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

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/msgdlg.h>
#endif

#include "milPanel.h"
#include <sqlite3.h>
#include "obdbase.h"

milPanel::milPanel( wxWindow* parent, obdbase* device, sqlite3* sql )
:
milBasePanel( parent )
{
	obd = device;
	db = sql;

	listDTCs->InsertColumn(0, _("DTC"), wxLIST_FORMAT_LEFT, -1);
	listDTCs->InsertColumn(1, _("Description"), wxLIST_FORMAT_LEFT, -1);
}

void milPanel::updateStatus ()
{
	int codeCount;
	obdbase::pidInfo* result;
	wxString na(_("N/A"));
	wxString status;
	wxString ennum;
	wxString time;
	wxString dist;

	codeCount = obd->obd_mil_status();

	if (obd->obd_is_connected() && (codeCount > 0)) {
		ennum.Printf(_T("%d"), codeCount);
		milEnnumerate->SetLabel(ennum);

		status = _("On");
		milStatus->SetLabel(status);

        obd->obd_pid_value(PID_MIL_TIME, result);
		time.Printf(_("%d mins"), result->resultMain);
		milTime->SetLabel(time);

        obd->obd_pid_value(PID_MIL_DIST, result);
		if (obd->obd_is_imperial()) {
			dist.Printf(_("%d miles"), result->resultMain);
		} else {
			dist.Printf(_("%d km"), result->resultMain);
		}
		milDist->SetLabel(dist);

		btnClear->Enable(true);

		this->updateErrors();
	} else {
		btnClear->Enable(false);
		milStatus->SetLabel(na);
		milEnnumerate->SetLabel(na);
		milDist->SetLabel(na);
		milTime->SetLabel(na);
		listDTCs->DeleteAllItems();
	}
}

void milPanel::onInit( wxInitDialogEvent& WXUNUSED(event) )
{

}

void milPanel::onClearClick( wxCommandEvent& WXUNUSED(event) )
{
	long style;
	wxString caption;

	wxString msg(_("Warning!\n\nYour vehicle is displaying the MIL for a reason."\
		"Do not reset the trouble codes unless you know what you are doing.\n\n" \
		"Are you sure you want to proceed?"));
	wxMessageDialog dialog(NULL, msg, _("Warning"), wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);

	int proceed = dialog.ShowModal();

	if (proceed == wxID_YES) {
		if (obd->obd_clear_dtc()) {
			msg = _("All error codes have been successfully cleared");
			style = wxOK | wxICON_INFORMATION;
			caption = _("DTC's Cleared");
		} else {
			msg = _("The ECU reported that it did not clear the error codes");
			style = wxOK | wxICON_ERROR;
			caption = _("Error");
		}

		wxMessageDialog clearDialog(NULL, msg, caption, style);
		clearDialog.ShowModal();
		this->updateStatus();
	}
}

void milPanel::enableButton(bool enable)
{
	if (enable == false) {
		buttonOld = btnClear->IsEnabled();
		btnClear->Enable(enable);
	} else {
		btnClear->Enable(buttonOld);
	}
}

void milPanel::updateErrors()
{
	wxArrayString errors;
	wxString desc;
	wxString sql(_T("SELECT * FROM dtcs WHERE dtc = ?1"));
	sqlite3_stmt *stmt;
	long itemIndex;
	char buf[100];

	// Get the reported error codes
	errors = obd->obd_mil_error_codes();

    // prepare the statement for the db
	if (sqlite3_prepare_v2(db, sql.mb_str(), -1, &stmt, NULL) == SQLITE_OK)
	{

		// process each error code in turn
		for (int i = 0; i < errors.GetCount(); i++) {
            // bind the error to the statement
            strcpy( buf, (const char*)errors.Item(i).mb_str(wxConvUTF8) );
            int result = sqlite3_bind_text(stmt, 1, buf, -1, SQLITE_STATIC);

            // the statement should only have one step
            // TODO: Change statement to support manufacturer codes
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // insert the code and description into the list control
                itemIndex = listDTCs->InsertItem(0, errors.Item(i));
                desc = wxString::FromUTF8((const char *)sqlite3_column_text(stmt, 3));
                listDTCs->SetItem(itemIndex, 1, desc);
            }

            // reset the sql
            sqlite3_reset(stmt);
		}

        // don't need the statement any more
		sqlite3_finalize(stmt);
	}
}

void milPanel::updateDevice(obdbase* device)
{
    obd = device;
}
