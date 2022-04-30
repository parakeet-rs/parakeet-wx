///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ui.h"

///////////////////////////////////////////////////////////////////////////
using namespace umd::ui_base;

uiMainAppFrame::uiMainAppFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_staticLabelFiles = new wxStaticText( this, wxID_ANY, _("Files:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabelFiles->Wrap( -1 );
	bSizer1->Add( m_staticLabelFiles, 0, wxALL|wxEXPAND, 4 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_fileListTable = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer5->Add( m_fileListTable, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_checkAutoDecrypt = new wxCheckBox( this, wxID_ANY, _("Auto decrypt on add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkAutoDecrypt, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	m_btnAddFile = new wxButton( this, wxID_ANY, _("Add files..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnAddFile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_btnAddDir = new wxButton( this, wxID_ANY, _("Add directory..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnAddDir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	m_btnProcess = new wxButton( this, wxID_ANY, _("Decrypt"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnProcess, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

uiMainAppFrame::~uiMainAppFrame()
{
}

Dialog1::Dialog1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );


	this->Centre( wxBOTH );
}

Dialog1::~Dialog1()
{
}
