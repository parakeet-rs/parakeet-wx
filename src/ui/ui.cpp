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

	m_propertyGrid2 = new wxPropertyGridManager(m_panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage1 = m_propertyGrid2->AddPage( _("Page"), wxNullBitmap );
	m_propGeneralCat1 = m_propertyGridPage1->Append( new wxPropertyCategory( _("Threading"), _("Threading") ) );
	m_generalThreadPoolSize = m_propertyGridPage1->Append( new wxIntProperty( _("Thread count"), _("Thread count") ) );
	m_propertyGridPage1->SetPropertyHelpString( m_generalThreadPoolSize, _("How many thread(s) to process the file queue.") );
	m_propertyGridItem32 = m_propertyGridPage1->Append( new wxPropertyCategory( _("Localization"), _("Localization") ) );
	m_generalLocale = m_propertyGridPage1->Append( new wxStringProperty( _("Locale Code"), _("Locale Code") ) );
	m_propertyGridPage1->SetPropertyHelpString( m_generalLocale, _("Language code. Use \"en\" for English, or zh_CN for Simplified Chinese.") );
	bSizer6->Add( m_propertyGrid2, 1, wxEXPAND, FromDIP(5) );


	m_panelGeneral->SetSizer( bSizer6 );
	m_panelGeneral->Layout();
	bSizer6->Fit( m_panelGeneral );
	m_notebookOptions->AddPage( m_panelGeneral, _("General"), false );
	m_panelKugou = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_propGridKugou = new wxPropertyGridManager(m_panelKugou, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage2 = m_propGridKugou->AddPage( _("Page"), wxNullBitmap );
	m_propertyGridItem3 = m_propertyGridPage2->Append( new wxPropertyCategory( _("KGM/VPR Decryption"), _("KGM/VPR Decryption") ) );
	m_kugouSlotKey1 = m_propertyGridPage2->Append( new wxLongStringProperty( _("Slot Key 1 (hex; 4 bytes)"), _("Slot Key 1 (hex; 4 bytes)") ) );
	m_propertyGridPage2->SetPropertyHelpString( m_kugouSlotKey1, _("Kugou uses a \"key-slot\" mechanism, where the key is specified by the key-slot stored in the encrypted file.") );
	m_kugouV4SlotKeyExpansionTable = m_propertyGridPage2->Append( new wxLongStringProperty( _("(v4) Slot key expansion table (hex)"), _("(v4) Slot key expansion table (hex)") ) );
	m_kugouV4FileKeyExpansionTable = m_propertyGridPage2->Append( new wxLongStringProperty( _("(v4) File key expansion table (hex)"), _("(v4) File key expansion table (hex)") ) );
	bSizer5->Add( m_propGridKugou, 1, wxEXPAND, FromDIP(5) );


	m_panelKugou->SetSizer( bSizer5 );
	m_panelKugou->Layout();
	bSizer5->Fit( m_panelKugou );
	m_notebookOptions->AddPage( m_panelKugou, _("Kugou"), false );
	m_panelKuwo = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid8 = new wxPropertyGridManager(m_panelKuwo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage3 = m_propertyGrid8->AddPage( _("Page"), wxNullBitmap );
	m_propertyGridItem36 = m_propertyGridPage3->Append( new wxPropertyCategory( _("Kuwo"), _("Kuwo") ) );
	m_kuwoKey = m_propertyGridPage3->Append( new wxLongStringProperty( _("Kuwo decryption table"), _("Kuwo decryption table") ) );
	m_propertyGridPage3->SetPropertyHelpString( m_kuwoKey, _("A 16-byte decryption key.") );
	bSizer12->Add( m_propertyGrid8, 1, wxEXPAND, FromDIP(5) );


	m_panelKuwo->SetSizer( bSizer12 );
	m_panelKuwo->Layout();
	bSizer12->Fit( m_panelKuwo );
	m_notebookOptions->AddPage( m_panelKuwo, _("Kuwo"), false );
	m_panelNetease = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid81 = new wxPropertyGridManager(m_panelNetease, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage4 = m_propertyGrid81->AddPage( _("Page"), wxNullBitmap );
	m_propertyGridItem361 = m_propertyGridPage4->Append( new wxPropertyCategory( _("NCM Decryption"), _("NCM Decryption") ) );
	m_neteaseNCMContentKeyAESKey = m_propertyGridPage4->Append( new wxLongStringProperty( _("AES key for NCM content key"), _("AES key for NCM content key") ) );
	m_propertyGridPage4->SetPropertyHelpString( m_neteaseNCMContentKeyAESKey, _("AES Key used for NCM Audio Content decryption.") );
	bSizer121->Add( m_propertyGrid81, 1, wxEXPAND, FromDIP(5) );


	m_panelNetease->SetSizer( bSizer121 );
	m_panelNetease->Layout();
	bSizer121->Fit( m_panelNetease );
	m_notebookOptions->AddPage( m_panelNetease, _("Netease"), false );
	m_panelQQMusic = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid41 = new wxPropertyGridManager(m_panelQQMusic, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage5 = m_propertyGrid41->AddPage( _("Page"), wxNullBitmap );
	m_propertyGridItem14 = m_propertyGridPage5->Append( new wxPropertyCategory( _("QMCv1"), _("QMCv1") ) );
	m_tencentStaticTable = m_propertyGridPage5->Append( new wxLongStringProperty( _("Static Key (hex; 128 bytes)"), _("Static Key (hex; 128 bytes)") ) );
	m_propertyGridPage5->SetPropertyHelpString( m_tencentStaticTable, _("128-Byte long encryption key. Optimized from 256-byte key.") );
	m_propertyGridItem35 = m_propertyGridPage5->Append( new wxPropertyCategory( _("QMCv2"), _("QMCv2") ) );
	m_tencentEKeySeed = m_propertyGridPage5->Append( new wxIntProperty( _("EKey derive seed"), _("EKey derive seed") ) );
	m_propertyGridPage5->SetPropertyHelpString( m_tencentEKeySeed, _("EKey seed, [0 ~ 255] inclusive.") );
	m_qmcEncV2Header = m_propertyGridPage5->Append( new wxPropertyCategory( _("QMCv2 - EncV2"), _("QMCv2 - EncV2") ) );
	m_encV2Stage1Key = m_propertyGridPage5->Append( new wxLongStringProperty( _("Mix key 1 (Stage 1 Key; 16 bytes)"), _("Mix key 1 (Stage 1 Key; 16 bytes)") ) );
	m_propertyGridPage5->SetPropertyHelpString( m_encV2Stage1Key, _("(EncV2) EKey Decryption Key, Stage 1, 16 bytes.") );
	m_encV2Stage2Key = m_propertyGridPage5->Append( new wxLongStringProperty( _("Mix key 2 (Stage 2 Key; 16 bytes)"), _("Mix key 2 (Stage 2 Key; 16 bytes)") ) );
	m_propertyGridPage5->SetPropertyHelpString( m_encV2Stage2Key, _("(EncV2) EKey Decryption Key, Stage 2, 16 bytes.") );
	bSizer9->Add( m_propertyGrid41, 1, wxEXPAND, FromDIP(5) );


	m_panelQQMusic->SetSizer( bSizer9 );
	m_panelQQMusic->Layout();
	bSizer9->Fit( m_panelQQMusic );
	m_notebookOptions->AddPage( m_panelQQMusic, _("QQ Music"), false );
	m_panelJoox = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxVERTICAL );

	m_propertyGridJoox = new wxPropertyGridManager(m_panelJoox, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage6 = m_propertyGridJoox->AddPage( _("Page"), wxNullBitmap );
	m_HeaderJoox = m_propertyGridPage6->Append( new wxPropertyCategory( _("Joox"), _("Joox") ) );
	m_jooxUUID = m_propertyGridPage6->Append( new wxStringProperty( _("Installation UUID"), _("Installation UUID") ) );
	m_propertyGridPage6->SetPropertyHelpString( m_jooxUUID, _("Installation ID generated on the initial app startup.") );
	m_jooxSalt = m_propertyGridPage6->Append( new wxLongStringProperty( _("Salt"), _("Salt") ) );
	m_propertyGridPage6->SetPropertyHelpString( m_jooxSalt, _("Salt is a hardcoded sequence of bytes, used to derive the AES decryption key.") );
	bSizer91->Add( m_propertyGridJoox, 1, wxEXPAND, FromDIP(5) );


	m_panelJoox->SetSizer( bSizer91 );
	m_panelJoox->Layout();
	bSizer91->Fit( m_panelJoox );
	m_notebookOptions->AddPage( m_panelJoox, _("Joox"), false );
	m_panelXimalaya = new wxPanel( m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid4 = new wxPropertyGridManager(m_panelXimalaya, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);

	m_propertyGridPage7 = m_propertyGrid4->AddPage( _("Page"), wxNullBitmap );
	m_propertyGridItem25 = m_propertyGridPage7->Append( new wxPropertyCategory( _("X2M"), _("X2M") ) );
	m_xmlyX2MContentKey = m_propertyGridPage7->Append( new wxLongStringProperty( _("X2M Content Key (hex; 4 bytes)"), _("X2M Content Key (hex; 4 bytes)") ) );
	m_propertyGridPage7->SetPropertyHelpString( m_xmlyX2MContentKey, _(".x2m Content Key.") );
	m_xmlyX2MScrambleTableParamInit = m_propertyGridPage7->Append( new wxFloatProperty( _("X2M Scramble Table Param - Init (0 ~ 1)"), _("X2M Scramble Table Param - Init (0 ~ 1)") ) );
	m_propertyGridPage7->SetPropertyHelpString( m_xmlyX2MScrambleTableParamInit, _("The initial value is used to generate Ximalaya Scramble Table.\nThis value is between 0 and 1, and up to 6 significant figures.") );
	m_xmlyX2MScrambleTableParamStep = m_propertyGridPage7->Append( new wxFloatProperty( _("X2M Scramble Table Param- Step (3.57 ~ 4)"), _("X2M Scramble Table Param- Step (3.57 ~ 4)") ) );
	m_propertyGridPage7->SetPropertyHelpString( m_xmlyX2MScrambleTableParamStep, _("The step value is used to generate Ximalaya Scramble Table.\nThis value is between 3.57 and 4, and up to 6 significant figures.") );
	m_propertyGridItem26 = m_propertyGridPage7->Append( new wxPropertyCategory( _("X3M"), _("X3M") ) );
	m_xmlyX3MContentKey = m_propertyGridPage7->Append( new wxLongStringProperty( _("X3M Content Key (hex; 32 bytes)"), _("X3M Content Key (hex; 32 bytes)") ) );
	m_propertyGridPage7->SetPropertyHelpString( m_xmlyX3MContentKey, _(".x3m Content Key.") );
	m_xmlyX3MScrambleTableParamInit = m_propertyGridPage7->Append( new wxFloatProperty( _("X3M Scramble Table Param - Init (0 ~ 1)"), _("X3M Scramble Table Param - Init (0 ~ 1)") ) );
	m_propertyGridPage7->SetPropertyHelpString( m_xmlyX3MScrambleTableParamInit, _("The initial value is used to generate Ximalaya Scramble Table.\nThis value is between 0 and 1, and up to 6 significant figures.") );
	m_xmlyX3MScrambleTableParamStep = m_propertyGridPage7->Append( new wxFloatProperty( _("X3M Scramble Table Param- Step (3.57 ~ 4)"), _("X3M Scramble Table Param- Step (3.57 ~ 4)") ) );
	m_propertyGridPage7->SetPropertyHelpString( m_xmlyX3MScrambleTableParamStep, _("The step value is used to generate Ximalaya Scramble Table.\nThis value is between 3.57 and 4, and up to 6 significant figures.") );
	bSizer7->Add( m_propertyGrid4, 1, wxEXPAND, FromDIP(5) );


	m_panelXimalaya->SetSizer( bSizer7 );
	m_panelXimalaya->Layout();
	bSizer7->Fit( m_panelXimalaya );
	m_notebookOptions->AddPage( m_panelXimalaya, _("Ximalaya"), true );

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
