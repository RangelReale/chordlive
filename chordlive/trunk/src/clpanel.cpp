#include "clpanel.h"
#include "clmain.h"

inline void CLPanel::onEraseBackground(wxEraseEvent &) {}

IMPLEMENT_CLASS(CLPanel, wxPanel)

BEGIN_EVENT_TABLE(CLPanel, wxPanel)
    EVT_PAINT(CLPanel::onPaint)
    EVT_ERASE_BACKGROUND(CLPanel::onEraseBackground)
	EVT_KEY_DOWN(CLPanel::onKeyDown)
END_EVENT_TABLE()

CLPanel::CLPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, 
	long style, const wxString& name) : wxPanel(parent, id, pos, size, style|wxWANTS_CHARS, name), startpos_(0) {
	song_ = new CLSong;

	//song_->LoadFromFile(wxT("M:\\prog\\personal\\chordlive\\songs\\Beatles - I Want to Hold Your Hand.cls"));
}

CLPanel::~CLPanel() 
{
	delete song_;
}

void CLPanel::onPaint(wxPaintEvent &) {
    wxBufferedPaintDC dc(this);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(GetClientRect());

	CLSongDraw sd(song_);
	sd.SetStartPos(startpos_);

	sd.Draw(dc, GetClientRect());
}

void CLPanel::onKeyDown(wxKeyEvent &event)
{
	if (event.GetKeyCode()==WXK_DOWN)
	{
		startpos_+=5;
		Refresh();
	} else if (event.GetKeyCode()==WXK_UP) {
		startpos_-=5;
		Refresh();
	} else if (event.GetKeyCode()==WXK_PAGEDOWN) {
		startpos_+=40;
		Refresh();
	} else if (event.GetKeyCode()==WXK_PAGEUP) {
		startpos_-=40;
		Refresh();
	} else if (event.GetKeyCode()==WXK_HOME) {
		startpos_=0;
		Refresh();
	}
	event.Skip();
}
