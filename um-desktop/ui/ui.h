///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
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
#include <wx/listbook.h>
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
			wxButton* m_btnAddFile;
			wxButton* m_btnAddDir;

			// Virtual event handlers, override them in your derived class
			virtual void uiMainAppFrameOnSize( wxSizeEvent& event ) { event.Skip(); }
			virtual void OnBtnClickOptions( wxCommandEvent& event ) { event.Skip(); }


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
			wxListbook* m_tabsOptions;
			wxPanel* m_panelKugou;
			wxPropertyGrid* m_propGridKugou;

			// Virtual event handlers, override them in your derived class
			virtual void OnActivate( wxActivateEvent& event ) { event.Skip(); }
			virtual void OnPropertyGridChanged_KuGou( wxPropertyGridEvent& event ) { event.Skip(); }


		public:

			uiOptionsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 381,279 ), long style = wxDEFAULT_DIALOG_STYLE );

			~uiOptionsDialog();

	};

} // namespace umd::ui_base

