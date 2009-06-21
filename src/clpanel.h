#ifndef H__CLPANEL__H
#define H__CLPANEL__H

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/image.h>
#include "clsong.h"

class CLPanel : public wxPanel {
    DECLARE_CLASS(CLPanel)
    DECLARE_EVENT_TABLE()
private:
	CLSong *song_;
	int startpos_;

    void onPaint(wxPaintEvent &event);
    void onEraseBackground(wxEraseEvent &event);
	void onKeyDown(wxKeyEvent &event);
public:
    CLPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxT("panel"));
    ~CLPanel();

	CLSong &GetSong() { return *song_; }
};



#endif // H__CLPANEL__H
