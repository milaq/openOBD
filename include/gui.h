///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/intl.h>

#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/aui/auibook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/listctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class obdBaseFrame
///////////////////////////////////////////////////////////////////////////////
class obdBaseFrame : public wxFrame 
{
	private:
	
	protected:
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu31;
		wxMenu* m_menu4;
		wxMenuItem* menuViewLog;
		wxMenuItem* menuViewMIL;
		wxMenuItem* menuViewPIDS;
		wxMenu* m_menu5;
		wxMenuItem* menuConnect;
		wxMenu* m_menu3;
		wxAuiNotebook* m_auinotebook1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onMenuExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuViewLog( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuViewMIL( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuViewPIDs( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuConnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuPrefs( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuAbout( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		obdBaseFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("openOBD"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~obdBaseFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class logBasePanel
///////////////////////////////////////////////////////////////////////////////
class logBasePanel : public wxPanel 
{
	private:
	
	protected:
		wxRichTextCtrl* m_richText1;
	
	public:
		
		logBasePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~logBasePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class milBasePanel
///////////////////////////////////////////////////////////////////////////////
class milBasePanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* milStatusText;
		wxStaticText* milStatus;
		wxStaticText* milEnnumerateText;
		wxStaticText* milEnnumerate;
		wxStaticText* milDistText;
		wxStaticText* milDist;
		wxStaticText* milTimeText;
		wxStaticText* milTime;
		wxButton* btnClear;
		wxListCtrl* listDTCs;
		wxListCtrl* listFreeze;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onInit( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void onClearClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		milBasePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~milBasePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dlgBaseOptions
///////////////////////////////////////////////////////////////////////////////
class dlgBaseOptions : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText9;
		wxComboBox* cmb_Port;
		wxCheckBox* check_Imperial;
		wxCheckBox* checkStartUp;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onOK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dlgBaseOptions( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~dlgBaseOptions();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class pidBasePanel
///////////////////////////////////////////////////////////////////////////////
class pidBasePanel : public wxPanel 
{
	private:
	
	protected:
		wxButton* btnRefresh;
		wxListCtrl* pidList;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onRefreshClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		pidBasePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~pidBasePanel();
	
};

#endif //__gui__
