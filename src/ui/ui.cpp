///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ui.h"

///////////////////////////////////////////////////////////////////////////
using namespace parakeet_wx::ui_base;

uiMainAppFrame::uiMainAppFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_staticLabelFiles = new wxStaticText( this, wxID_ANY, _("Logs:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticLabelFiles->Wrap( -1 );
	bSizer1->Add( m_staticLabelFiles, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, FromDIP(8) );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_decryptLogs = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT|wxLC_VRULES );
	bSizer5->Add( m_decryptLogs, 1, wxALL|wxEXPAND, FromDIP(0) );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_btnOptions = new wxButton( this, wxID_ANY, _("&Options..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnOptions, 0, 0, FromDIP(5) );

	m_btnClearLogs = new wxButton( this, wxID_ANY, _("&Clear Logs"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnClearLogs, 0, wxLEFT, FromDIP(8) );


	bSizer6->Add( FromDIP(0), FromDIP(0), 1, wxEXPAND, FromDIP(5) );

	m_btnProcess = new wxButton( this, wxID_ANY, _("Process"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnProcess, 0, 0, FromDIP(5) );


	bSizer6->Add( FromDIP(0), FromDIP(0), 1, wxEXPAND, FromDIP(5) );

	m_btnAddFile = new wxButton( this, wxID_ANY, _("Decrypt &file..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnAddFile, 0, 0, FromDIP(5) );

	m_btnAddDir = new wxButton( this, wxID_ANY, _("&Decrypt directory..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_btnAddDir, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, FromDIP(8) );


	bSizer5->Add( bSizer6, 0, wxBOTTOM|wxEXPAND|wxTOP, FromDIP(8) );


	bSizer1->Add( bSizer5, 1, wxEXPAND|wxLEFT|wxRIGHT, FromDIP(8) );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( uiMainAppFrame::uiMainAppFrameOnSize ) );
	m_btnOptions->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnBtnClickOptions ), NULL, this );
	m_btnClearLogs->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_ClearLogs ), NULL, this );
	m_btnProcess->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_ProcessFiles ), NULL, this );
	m_btnAddFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_AddFile ), NULL, this );
	m_btnAddDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_AddDirectory ), NULL, this );
}

uiMainAppFrame::~uiMainAppFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( uiMainAppFrame::uiMainAppFrameOnSize ) );
	m_btnOptions->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnBtnClickOptions ), NULL, this );
	m_btnClearLogs->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_ClearLogs ), NULL, this );
	m_btnProcess->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_ProcessFiles ), NULL, this );
	m_btnAddFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_AddFile ), NULL, this );
	m_btnAddDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiMainAppFrame::OnButtonClick_AddDirectory ), NULL, this );

}

uiOptionsDialog::uiOptionsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_notebookOptions = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid2 = new wxPropertyGrid(m_panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propGeneralCat1 = m_propertyGrid2->Append( new wxPropertyCategory( _("Threading"), _("Threading") ) );
	m_generalThreadPoolSize = m_propertyGrid2->Append( new wxIntProperty( _("Thread count"), _("Thread count") ) );
	m_propertyGrid2->SetPropertyHelpString( m_generalThreadPoolSize, _(" a b c") );
	m_generalLocale = m_propertyGrid2->Append( new wxStringProperty( _("Locale Code"), _("Locale Code") ) );
	bSizer6->Add( m_propertyGrid2, 1, wxEXPAND, FromDIP(5) );


	m_panelGeneral->SetSizer( bSizer6 );
	m_panelGeneral->Layout();
	bSizer6->Fit( m_panelGeneral );
	m_notebookOptions->AddPage( m_panelGeneral, _("General"), true );
	m_panelKugou = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_propGridKugou = new wxPropertyGrid(m_panelKugou, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propertyGridItem3 = m_propGridKugou->Append( new wxPropertyCategory( _("KGM/VPR Decryption"), _("KGM/VPR Decryption") ) );
	m_kugouSlotKey1 = m_propGridKugou->Append( new wxLongStringProperty( _("Slot Key 1 (hex; 4 bytes)"), _("Slot Key 1 (hex; 4 bytes)") ) );
	m_kugouV4SlotKeyExpansionTable = m_propGridKugou->Append( new wxLongStringProperty( _("(v4) Slot key expansion table (hex)"), _("(v4) Slot key expansion table (hex)") ) );
	m_kugouV4FileKeyExpansionTable = m_propGridKugou->Append( new wxLongStringProperty( _("(v4) File key expansion table (hex)"), _("(v4) File key expansion table (hex)") ) );
	bSizer5->Add( m_propGridKugou, 1, wxEXPAND, FromDIP(5) );


	m_panelKugou->SetSizer( bSizer5 );
	m_panelKugou->Layout();
	bSizer5->Fit( m_panelKugou );
	m_notebookOptions->AddPage( m_panelKugou, _("Kugou"), false );
	m_panelKuwo = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid8 = new wxPropertyGrid(m_panelKuwo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propertyGridItem36 = m_propertyGrid8->Append( new wxPropertyCategory( _("Kuwo"), _("Kuwo") ) );
	m_kuwoKey = m_propertyGrid8->Append( new wxLongStringProperty( _("Kuwo decryption table"), _("Kuwo decryption table") ) );
	bSizer12->Add( m_propertyGrid8, 1, wxEXPAND, FromDIP(5) );


	m_panelKuwo->SetSizer( bSizer12 );
	m_panelKuwo->Layout();
	bSizer12->Fit( m_panelKuwo );
	m_notebookOptions->AddPage( m_panelKuwo, _("Kuwo"), false );
	m_panelNetease = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid81 = new wxPropertyGrid(m_panelNetease, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propertyGridItem361 = m_propertyGrid81->Append( new wxPropertyCategory( _("NCM Decryption"), _("NCM Decryption") ) );
	m_neteaseNCMContentKeyAESKey = m_propertyGrid81->Append( new wxLongStringProperty( _("AES key for NCM content key"), _("AES key for NCM content key") ) );
	bSizer121->Add( m_propertyGrid81, 1, wxEXPAND, FromDIP(5) );


	m_panelNetease->SetSizer( bSizer121 );
	m_panelNetease->Layout();
	bSizer121->Fit( m_panelNetease );
	m_notebookOptions->AddPage( m_panelNetease, _("Netease"), false );
	m_panelQQMusic = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid41 = new wxPropertyGrid(m_panelQQMusic, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propertyGridItem14 = m_propertyGrid41->Append( new wxPropertyCategory( _("QMCv1"), _("QMCv1") ) );
	m_tencentStaticTable = m_propertyGrid41->Append( new wxLongStringProperty( _("Static Key (hex; 128 bytes)"), _("Static Key (hex; 128 bytes)") ) );
	m_propertyGridItem35 = m_propertyGrid41->Append( new wxPropertyCategory( _("QMCv2"), _("QMCv2") ) );
	m_tencentEKeySeed = m_propertyGrid41->Append( new wxIntProperty( _("EKey derive seed"), _("EKey derive seed") ) );
	m_qmcEncV2Header = m_propertyGrid41->Append( new wxPropertyCategory( _("QMCv2 - EncV2"), _("QMCv2 - EncV2") ) );
	m_encV2Stage1Key = m_propertyGrid41->Append( new wxLongStringProperty( _("Mix key 1 (Stage 1 Key; 16 bytes)"), _("Mix key 1 (Stage 1 Key; 16 bytes)") ) );
	m_encV2Stage2Key = m_propertyGrid41->Append( new wxLongStringProperty( _("Mix key 2 (Stage 2 Key; 16 bytes)"), _("Mix key 2 (Stage 2 Key; 16 bytes)") ) );
	bSizer9->Add( m_propertyGrid41, 1, wxEXPAND, FromDIP(5) );


	m_panelQQMusic->SetSizer( bSizer9 );
	m_panelQQMusic->Layout();
	bSizer9->Fit( m_panelQQMusic );
	m_notebookOptions->AddPage( m_panelQQMusic, _("QQ Music"), false );
	m_panelJoox = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxVERTICAL );

	m_propertyGridJoox = new wxPropertyGrid(m_panelJoox, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_HeaderJoox = m_propertyGridJoox->Append( new wxPropertyCategory( _("Joox"), _("Joox") ) );
	m_jooxUUID = m_propertyGridJoox->Append( new wxStringProperty( _("Installation UUID"), _("Installation UUID") ) );
	m_jooxSalt = m_propertyGridJoox->Append( new wxLongStringProperty( _("Salt"), _("Salt") ) );
	bSizer91->Add( m_propertyGridJoox, 1, wxEXPAND, FromDIP(5) );


	m_panelJoox->SetSizer( bSizer91 );
	m_panelJoox->Layout();
	bSizer91->Fit( m_panelJoox );
	m_notebookOptions->AddPage( m_panelJoox, _("Joox"), false );
	m_panelXimalaya = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid4 = new wxPropertyGrid(m_panelXimalaya, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propertyGridItem25 = m_propertyGrid4->Append( new wxPropertyCategory( _("X2M"), _("X2M") ) );
	m_xmlyX2MContentKey = m_propertyGrid4->Append( new wxLongStringProperty( _("X2M Content Key (hex; 4 bytes)"), _("X2M Content Key (hex; 4 bytes)") ) );
	m_xmlyX2MScrambleTable = m_propertyGrid4->Append( new wxLongStringProperty( _("X2M Scramble Table (hex; 1024 u16)"), _("X2M Scramble Table (hex; 1024 u16)") ) );
	m_propertyGridItem26 = m_propertyGrid4->Append( new wxPropertyCategory( _("X3M"), _("X3M") ) );
	m_xmlyX3MContentKey = m_propertyGrid4->Append( new wxLongStringProperty( _("X3M Content Key (hex; 32 bytes)"), _("X3M Content Key (hex; 32 bytes)") ) );
	m_xmlyX3MScrambleTable = m_propertyGrid4->Append( new wxLongStringProperty( _("X3M Scramble Table (hex; 1024 u16)"), _("X3M Scramble Table (hex; 1024 u16)") ) );
	bSizer7->Add( m_propertyGrid4, 1, wxEXPAND, FromDIP(5) );


	m_panelXimalaya->SetSizer( bSizer7 );
	m_panelXimalaya->Layout();
	bSizer7->Fit( m_panelXimalaya );
	m_notebookOptions->AddPage( m_panelXimalaya, _("Ximalaya"), false );

	bSizer4->Add( m_notebookOptions, 1, wxEXPAND | wxALL, FromDIP(5) );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );


	bSizer8->Add( FromDIP(0), FromDIP(0), 1, wxEXPAND, FromDIP(5) );

	m_btnCancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_btnCancel, 0, wxALL, FromDIP(5) );

	m_btnOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );

	m_btnOK->SetDefault();
	bSizer8->Add( m_btnOK, 0, wxALL, FromDIP(5) );


	bSizer4->Add( bSizer8, 0, wxBOTTOM|wxEXPAND|wxRIGHT, FromDIP(8) );


	this->SetSizer( bSizer4 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( uiOptionsDialog::OnActivate ) );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiOptionsDialog::OnButtonClick_Cancel ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiOptionsDialog::OnButtonClick_OK ), NULL, this );
}

uiOptionsDialog::~uiOptionsDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( uiOptionsDialog::OnActivate ) );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiOptionsDialog::OnButtonClick_Cancel ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( uiOptionsDialog::OnButtonClick_OK ), NULL, this );

}
