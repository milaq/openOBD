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
    #include <wx/wx.h>
#endif 

#include "main.h"
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "obdFrame.h"


IMPLEMENT_APP(wxWidgetsApp)

wxWidgetsApp::wxWidgetsApp()
{
}

wxWidgetsApp::~wxWidgetsApp()
{
}

bool wxWidgetsApp::OnInit()
{

	// localisation
	wxLocale m_locale;
	int sys_lang = wxLocale::GetSystemLanguage();
    if( sys_lang != wxLANGUAGE_DEFAULT )
    {
        m_locale.Init(sys_lang);                        	// set custom locale
        m_locale.AddCatalogLookupPathPrefix(_T("locale"));  // set "locale" prefix
        m_locale.AddCatalog(_T("openobd"));                 // our private domain
        m_locale.AddCatalog(_T("wxstd"));                		// wx common domain is default
    }

    // app name and suchlike
    SetVendorName(_T("openOBD"));
    SetAppName(_T("openOBD"));

    // create our data dir if necessary
    wxFileName dataDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
    if (!dataDir.DirExists()) {
        dataDir.Mkdir();
    }

    // wxConfig stuff
#if defined (WIN32)
    wxConfigBase *pConfig = new wxConfig(GetAppName(), GetVendorName(), wxEmptyString, wxEmptyString, wxCONFIG_USE_SUBDIR | wxCONFIG_USE_LOCAL_FILE);
#else
	wxConfigBase *pConfig = new wxConfig(GetAppName(), GetVendorName(), wxEmptyString, wxEmptyString, wxCONFIG_USE_SUBDIR | wxCONFIG_USE_LOCAL_FILE, wxConvUTF8);
#endif
    wxConfigBase::Set(pConfig);

	// normal init stuff
    obdFrame* frame = new obdFrame( (wxWindow*)NULL );
    frame->Show();
    SetTopWindow( frame );
    return true;
}

int wxWidgetsApp::OnExit()
{
    // delete the wxConfig object
    delete wxConfig::Set((wxConfigBase *) NULL);

    return 0;
}
