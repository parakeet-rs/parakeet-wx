///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


/// HiDPI polyfill
#include <wx/version.h> 
#if !wxCHECK_VERSION(3, 1, 0) && !defined(FromDIP)
#define FromDIP(x) (x)
#endif
///

namespace parakeet_wx::ui_base
{

	///////////////////////////////////////////////////////////////////////////////
	/// Class uiMainAppFrame
	///////////////////////////////////////////////////////////////////////////////
	class uiMainAppFrame : public wxFrame
	{
		private:

		protected:
			wxStaticText* m_staticLabelFiles;
			wxListCtrl* m_decryptLogs;
			wxButton* m_btnOptions;
			wxButton* m_btnClearLogs;
			wxButton* m_btnProcess;
			wxButton* m_btnAddFile;
			wxButton* m_btnAddDir;

			// Virtual event handlers, override them in your derived class
			virtual void uiMainAppFrameOnSize( wxSizeEvent& event ) { event.Skip(); }
			virtual void OnBtnClickOptions( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnButtonClick_ClearLogs( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnButtonClick_ProcessFiles( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnButtonClick_AddFile( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnButtonClick_AddDirectory( wxCommandEvent& event ) { event.Skip(); }


		public:

			uiMainAppFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Parakeet Desktop"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 679,362 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~uiMainAppFrame();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class uiOptionsDialog
	///////////////////////////////////////////////////////////////////////////////
	class uiOptionsDialog : public wxDialog
	{
		private:
			wxPGProperty* m_propertyGridItem32;

		protected:
			wxNotebook* m_notebookOptions;
			wxPanel* m_panelGeneral;
			wxPropertyGridManager* m_propertyGrid2;
			wxPropertyGridPage* m_propertyGridPage1;
			wxPGProperty* m_propGeneralCat1;
			wxPGProperty* m_generalThreadPoolSize;
			wxPGProperty* m_generalLocale;
			wxPanel* m_panelKugou;
			wxPropertyGridManager* m_propGridKugou;
			wxPropertyGridPage* m_propertyGridPage2;
			wxPGProperty* m_propertyGridItem3;
			wxPGProperty* m_kugouSlotKey1;
			wxPGProperty* m_kugouV4SlotKeyExpansionTable;
			wxPGProperty* m_kugouV4FileKeyExpansionTable;
			wxPanel* m_panelKuwo;
			wxPropertyGridManager* m_propertyGrid8;
			wxPropertyGridPage* m_propertyGridPage3;
			wxPGProperty* m_propertyGridItem36;
			wxPGProperty* m_kuwoKey;
			wxPanel* m_panelNetease;
			wxPropertyGridManager* m_propertyGrid81;
			wxPropertyGridPage* m_propertyGridPage4;
			wxPGProperty* m_propertyGridItem361;
			wxPGProperty* m_neteaseNCMContentKeyAESKey;
			wxPanel* m_panelQQMusic;
			wxPropertyGridManager* m_propertyGrid41;
			wxPropertyGridPage* m_propertyGridPage5;
			wxPGProperty* m_propertyGridItem14;
			wxPGProperty* m_tencentStaticTable;
			wxPGProperty* m_propertyGridItem35;
			wxPGProperty* m_tencentEKeySeed;
			wxPGProperty* m_qmcEncV2Header;
			wxPGProperty* m_encV2Stage1Key;
			wxPGProperty* m_encV2Stage2Key;
			wxPanel* m_panelJoox;
			wxPropertyGridManager* m_propertyGridJoox;
			wxPropertyGridPage* m_propertyGridPage6;
			wxPGProperty* m_HeaderJoox;
			wxPGProperty* m_jooxUUID;
			wxPGProperty* m_jooxSalt;
			wxPanel* m_panelXimalaya;
			wxPropertyGridManager* m_propertyGrid4;
			wxPropertyGridPage* m_propertyGridPage7;
			wxPGProperty* m_propertyGridItem25;
			wxPGProperty* m_xmlyX2MContentKey;
			wxPGProperty* m_xmlyX2MScrambleTableParamInit;
			wxPGProperty* m_xmlyX2MScrambleTableParamStep;
			wxPGProperty* m_propertyGridItem26;
			wxPGProperty* m_xmlyX3MContentKey;
			wxPGProperty* m_xmlyX3MScrambleTableParamInit;
			wxPGProperty* m_xmlyX3MScrambleTableParamStep;
			wxButton* m_btnCancel;
			wxButton* m_btnOK;

			// Virtual event handlers, override them in your derived class
			virtual void OnActivate( wxActivateEvent& event ) { event.Skip(); }
			virtual void OnButtonClick_Cancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnButtonClick_OK( wxCommandEvent& event ) { event.Skip(); }


		public:

			uiOptionsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 642,407 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

			~uiOptionsDialog();

	};

} // namespace parakeet_wx::ui_base

