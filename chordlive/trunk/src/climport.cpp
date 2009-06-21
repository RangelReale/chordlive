#include "climport.h"
#include <wx/statline.h>

IMPLEMENT_CLASS(CLImport, wxFrame)

BEGIN_EVENT_TABLE(CLImport, wxDialog)
	EVT_TEXT(ID_TITLE, CLImport::OnTextChanged)
	EVT_TEXT(ID_ARTIST, CLImport::OnTextChanged)
	EVT_TEXT(ID_TEXT, CLImport::OnLyricsChanged)
END_EVENT_TABLE()

CLImport::CLImport(wxWindow* parent) {
    Create(parent, wxID_ANY, wxT("ChordLive Import"), wxDefaultPosition,
           wxDefaultSize, wxDEFAULT_FRAME_STYLE);

	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// TITLE
	topsizer->Add(new wxStaticText(this, wxID_ANY, _("Title")), 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);
	
	wxTextCtrl *title = new wxTextCtrl(this, ID_TITLE, wxEmptyString, wxDefaultPosition, wxSize(200, -1));
	topsizer->Add(title, 0, wxALL|wxGROW, 3);

	// ARTIST
	topsizer->Add(new wxStaticText(this, wxID_ANY, _("Artist")), 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);
	
	wxTextCtrl *artist = new wxTextCtrl(this, ID_ARTIST, wxEmptyString, wxDefaultPosition, wxSize(200, -1));
	topsizer->Add(artist, 0, wxALL|wxGROW, 3);

	// TEXT SIZER
	wxFlexGridSizer *textsizer = new wxFlexGridSizer(2, 2);
	textsizer->AddGrowableRow(1, 1);
	textsizer->AddGrowableCol(0, 1);
	textsizer->AddGrowableCol(1, 1);
	textsizer->SetFlexibleDirection(wxBOTH);
	topsizer->Add(textsizer, 1, wxALL|wxGROW, 3);

	// TEXT
	textsizer->Add(new wxStaticText(this, wxID_ANY, _("Lyrics Text")), 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

	// PREVIEW
	textsizer->Add(new wxStaticText(this, wxID_ANY, _("Preview")), 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

	// TEXT
	wxTextCtrl *text = new wxTextCtrl(this, ID_TEXT, wxEmptyString, wxDefaultPosition, wxSize(400, 300), wxTE_MULTILINE);
	textsizer->Add(text, 1, wxALL|wxGROW, 3);

	// PREVIEW
	clpanel_ = new CLPanel(this, ID_PREVIEW, wxDefaultPosition, wxSize(400, 300));
	textsizer->Add(clpanel_, 1, wxALL|wxGROW, 3);

    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    topsizer->Add(line, 0, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    topsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}

void CLImport::OnTextChanged(wxCommandEvent &event)
{
	clpanel_->GetSong().SetTitle( ((wxTextCtrl*)FindWindow(ID_TITLE))->GetValue() );
	clpanel_->GetSong().SetArtist( ((wxTextCtrl*)FindWindow(ID_ARTIST))->GetValue() );
	clpanel_->Refresh();
}

void CLImport::OnLyricsChanged(wxCommandEvent &event)
{
	clpanel_->GetSong().Parse( ((wxTextCtrl*)FindWindow(ID_TEXT))->GetValue() );
	clpanel_->Refresh();
}
