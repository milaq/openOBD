///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

obdBaseFrame::obdBaseFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_statusBar1 = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* menuExport;
	menuExport = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Export Log...") ) , _("Export the current log to a file"), wxITEM_NORMAL );
	m_menu1->Append( menuExport );
	
	wxMenuItem* menuQuit;
	menuQuit = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Quit") ) + wxT('\t') + wxT("ALT + F4"), _("Close the openOBD application"), wxITEM_NORMAL );
	m_menu1->Append( menuQuit );
	
	m_menubar1->Append( m_menu1, _("&File") );
	
	m_menu31 = new wxMenu();
	m_menubar1->Append( m_menu31, _("&Edit") );
	
	m_menu4 = new wxMenu();
	menuViewLog = new wxMenuItem( m_menu4, wxID_ANY, wxString( _("&Log") ) , _("Display the raw log of the OBD-II device conversation"), wxITEM_CHECK );
	m_menu4->Append( menuViewLog );
	menuViewLog->Check( true );
	
	menuViewMIL = new wxMenuItem( m_menu4, wxID_ANY, wxString( _("&MIL Status") ) , _("Display MIL details"), wxITEM_CHECK );
	m_menu4->Append( menuViewMIL );
	menuViewMIL->Enable( false );
	
	menuViewPIDS = new wxMenuItem( m_menu4, wxID_ANY, wxString( _("PIDs") ) , _("Display all mode 1 PIDs"), wxITEM_CHECK );
	m_menu4->Append( menuViewPIDS );
	menuViewPIDS->Enable( false );
	
	m_menubar1->Append( m_menu4, _("&View") );
	
	m_menu5 = new wxMenu();
	menuConnect = new wxMenuItem( m_menu5, wxID_ANY, wxString( _("&Connect to interface") ) , _("Connect to OBD-II interface"), wxITEM_NORMAL );
	m_menu5->Append( menuConnect );
	
	wxMenuItem* menuPrefs;
	menuPrefs = new wxMenuItem( m_menu5, wxID_ANY, wxString( _("Preferences") ) , _("Change preferences"), wxITEM_NORMAL );
	m_menu5->Append( menuPrefs );
	
	m_menubar1->Append( m_menu5, _("&Tools") );
	
	m_menu3 = new wxMenu();
	wxMenuItem* menuAbout;
	menuAbout = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("&About") ) + wxT('\t') + wxT("F1"), _("Display copyright and author information"), wxITEM_NORMAL );
	m_menu3->Append( menuAbout );
	
	m_menubar1->Append( m_menu3, _("&Help") );
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_auinotebook1 = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT );
	
	bSizer1->Add( m_auinotebook1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( menuExport->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuExport ) );
	this->Connect( menuQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuQuit ) );
	this->Connect( menuViewLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuViewLog ) );
	this->Connect( menuViewMIL->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuViewMIL ) );
	this->Connect( menuViewPIDS->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuViewPIDs ) );
	this->Connect( menuConnect->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuConnect ) );
	this->Connect( menuPrefs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuPrefs ) );
	this->Connect( menuAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuAbout ) );
}

obdBaseFrame::~obdBaseFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuExport ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuViewLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuViewMIL ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuViewPIDs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuConnect ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuPrefs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( obdBaseFrame::onMenuAbout ) );
}

logBasePanel::logBasePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_richText1 = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer2->Add( m_richText1, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
}

logBasePanel::~logBasePanel()
{
}

milBasePanel::milBasePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Check Engine") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	milStatusText = new wxStaticText( this, wxID_ANY, _("MIL Status:"), wxDefaultPosition, wxDefaultSize, 0 );
	milStatusText->Wrap( -1 );
	fgSizer1->Add( milStatusText, 0, wxALL, 5 );
	
	milStatus = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	milStatus->Wrap( -1 );
	fgSizer1->Add( milStatus, 0, wxALL, 5 );
	
	milEnnumerateText = new wxStaticText( this, wxID_ANY, _("No of Error Codes:"), wxDefaultPosition, wxDefaultSize, 0 );
	milEnnumerateText->Wrap( -1 );
	fgSizer1->Add( milEnnumerateText, 0, wxALL, 5 );
	
	milEnnumerate = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	milEnnumerate->Wrap( -1 );
	fgSizer1->Add( milEnnumerate, 0, wxALL, 5 );
	
	milDistText = new wxStaticText( this, wxID_ANY, _("Distance travelled when lit:"), wxDefaultPosition, wxDefaultSize, 0 );
	milDistText->Wrap( -1 );
	fgSizer1->Add( milDistText, 0, wxALL, 5 );
	
	milDist = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	milDist->Wrap( -1 );
	fgSizer1->Add( milDist, 0, wxALL, 5 );
	
	milTimeText = new wxStaticText( this, wxID_ANY, _("Time run when lit:"), wxDefaultPosition, wxDefaultSize, 0 );
	milTimeText->Wrap( -1 );
	fgSizer1->Add( milTimeText, 0, wxALL, 5 );
	
	milTime = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	milTime->Wrap( -1 );
	fgSizer1->Add( milTime, 0, wxALL, 5 );
	
	btnClear = new wxButton( this, wxID_ANY, _("Clear Codes"), wxDefaultPosition, wxDefaultSize, 0 );
	btnClear->Enable( false );
	
	fgSizer1->Add( btnClear, 0, wxALL, 5 );
	
	sbSizer2->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	bSizer3->Add( sbSizer2, 0, wxALIGN_LEFT|wxALIGN_TOP, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	listDTCs = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer4->Add( listDTCs, 1, wxALL|wxEXPAND, 5 );
	
	listFreeze = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer4->Add( listFreeze, 1, wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( milBasePanel::onInit ) );
	btnClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( milBasePanel::onClearClick ), NULL, this );
}

milBasePanel::~milBasePanel()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( milBasePanel::onInit ) );
	btnClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( milBasePanel::onClearClick ), NULL, this );
}

dlgBaseOptions::dlgBaseOptions( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer7->Add( m_staticText9, 0, wxALL, 5 );
	
	cmb_Port = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN ); 
	bSizer7->Add( cmb_Port, 0, wxALL, 5 );
	
	bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );
	
	check_Imperial = new wxCheckBox( this, wxID_ANY, _("Use Imperial measurements"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( check_Imperial, 0, wxALL, 5 );
	
	checkStartUp = new wxCheckBox( this, wxID_ANY, _("Try to silently connect at Startup"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( checkStartUp, 0, wxALL, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer5->Add( m_sdbSizer2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizer2OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgBaseOptions::onOK ), NULL, this );
}

dlgBaseOptions::~dlgBaseOptions()
{
	// Disconnect Events
	m_sdbSizer2OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlgBaseOptions::onOK ), NULL, this );
}

pidBasePanel::pidBasePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	btnRefresh = new wxButton( this, wxID_ANY, _("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( btnRefresh, 0, wxALL, 5 );
	
	pidList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	bSizer7->Add( pidList, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	// Connect Events
	btnRefresh->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( pidBasePanel::onRefreshClick ), NULL, this );
}

pidBasePanel::~pidBasePanel()
{
	// Disconnect Events
	btnRefresh->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( pidBasePanel::onRefreshClick ), NULL, this );
}
