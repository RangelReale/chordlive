#ifndef H__CLIMPORT__H
#define H__CLIMPORT__H

#include <wx/wx.h>
#include "clpanel.h"

class CLImport : public wxDialog {
    DECLARE_CLASS(CLImport)
    DECLARE_EVENT_TABLE()
private:
	CLPanel *clpanel_;

	void OnTextChanged(wxCommandEvent &event);
	void OnLyricsChanged(wxCommandEvent &event);
public:
	enum {
		ID_TITLE,
		ID_ARTIST,
		ID_TEXT,
		ID_PREVIEW,
	};

    CLImport(wxWindow* parent);
};

#endif // H__CLIMPORT__H
