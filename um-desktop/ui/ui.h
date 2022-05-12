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
#include <wx/propgrid/advprops.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace umd::ui_base
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

			uiMainAppFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("um Desktop"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 679,362 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~uiMainAppFrame();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class uiOptionsDialog
	///////////////////////////////////////////////////////////////////////////////
	class uiOptionsDialog : public wxDialog
	{
		private:

		protected:
			wxNotebook* m_notebookOptions;
			wxPanel* m_panelGeneral;
			wxPropertyGrid* m_propertyGrid2;
			wxPGProperty* m_propGeneralCat1;
			wxPGProperty* m_generalThreadPoolSize;
			wxPGProperty* m_generalLocale;
			wxPanel* m_panelKugou;
			wxPropertyGrid* m_propGridKugou;
			wxPGProperty* m_propertyGridItem3;
			wxPGProperty* m_kugouT1;
			wxPGProperty* m_kugouT2;
			wxPGProperty* m_kugouV2;
			wxPGProperty* m_propertyGridItem6;
			wxPGProperty* m_kugouVPR;
			wxPanel* m_panelKuwo;
			wxPropertyGrid* m_propertyGrid8;
			wxPGProperty* m_propertyGridItem36;
			wxPGProperty* m_kuwoKey;
			wxPanel* m_panelQQMusic;
			wxPropertyGrid* m_propertyGrid41;
			wxPGProperty* m_propertyGridItem14;
			wxPGProperty* m_tencentStaticTable;
			wxPGProperty* m_propertyGridItem35;
			wxPGProperty* m_tencentEKeySeed;
			wxPanel* m_panelJoox;
			wxPropertyGrid* m_propertyGridJoox;
			wxPGProperty* m_HeaderJoox;
			wxPGProperty* m_jooxUUID;
			wxPGProperty* m_jooxSalt;
			wxPanel* m_panelXimalaya;
			wxPropertyGrid* m_propertyGrid4;
			wxPGProperty* m_propertyGridItem25;
			wxPGProperty* m_xmlyX2MContentKey;
			wxPGProperty* m_xmlyX2MScrambleTable;
			wxPGProperty* m_propertyGridItem26;
			wxPGProperty* m_xmlyX3MContentKey;
			wxPGProperty* m_xmlyX3MScrambleTable;
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

} // namespace umd::ui_base

