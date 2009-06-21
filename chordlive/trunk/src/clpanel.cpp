#include "clpanel.h"
#include "clmain.h"

inline void CLPanel::onEraseBackground(wxEraseEvent &) {}

IMPLEMENT_CLASS(CLPanel, wxPanel)

BEGIN_EVENT_TABLE(CLPanel, wxPanel)
    EVT_PAINT(CLPanel::onPaint)
    EVT_ERASE_BACKGROUND(CLPanel::onEraseBackground)
END_EVENT_TABLE()

CLPanel::CLPanel(wxWindow *parent) : wxPanel(parent, CLMain::ID_PANEL) {
	song_ = new CLSong;

	song_->LoadFromFile(wxT("M:\\prog\\personal\\chordlive\\songs\\Beatles - I Want to Hold Your Hand.cls"));
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

	song_->Draw(dc, GetClientRect());
}
