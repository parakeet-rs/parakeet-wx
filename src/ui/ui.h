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
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace ui
{

	///////////////////////////////////////////////////////////////////////////////
	/// Class uiMainAppFrame
	///////////////////////////////////////////////////////////////////////////////
	class uiMainAppFrame : public wxFrame
	{
		private:

		protected:
			wxButton* m_button1;
			wxTextCtrl* m_textCtrl1;

		public:

			uiMainAppFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("um Desktop"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~uiMainAppFrame();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class Dialog1
	///////////////////////////////////////////////////////////////////////////////
	class Dialog1 : public wxDialog
	{
		private:

		protected:

		public:

			Dialog1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

			~Dialog1();

	};

} // namespace ui

