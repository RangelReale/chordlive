#include "clsong.h"

/****
 *
 * CLSongLine
 *
 ***/
CLSongLine::CLSongLine()
{

}

CLSongLine::CLSongLine(linekind_t linekind, const wxString &line, const wxString &chords) :
	linekind_(linekind), line_(line), chords_(chords)
{

}

CLSongLine::~CLSongLine()
{

}


/****
 *
 * CLSong
 *
 ***/
CLSong::CLSong()
{
	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT(""), wxT("C  D    C  D    C  D"))));

	lines_.push_back(boost::shared_ptr<CLSongLine>(new CLSongLine(CLSongLine::LK_SPACE)));

	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT("Oh yeah I'll tell you somenthing"), wxT("G                      D"))));
	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT("I think you'll understand"), wxT("Em                   B"))));
	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT("When I say that something"), wxT("G                   D"))));
	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT("I want to hold your hand"), wxT("Em                   B"))));

	lines_.push_back(boost::shared_ptr<CLSongLine>(new CLSongLine(CLSongLine::LK_SPACE)));

	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT("I want to hold your hand"), wxT("C         D         G     Em"))));
	lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT("I want to hold your hand"), wxT("C         D         G"))));

	lines_.push_back(boost::shared_ptr<CLSongLine>(new CLSongLine(CLSongLine::LK_SPACE)));
}

CLSong::~CLSong()
{

}

void CLSong::Draw(wxDC &dc, const wxRect &rect)
{
	wxFont linefont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont chordfont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	int spos=0;
	for (songlist_t::const_iterator i=lines_.begin(); i!=lines_.end(); i++)
	{
		switch ((*i)->GetLineKind()) {
			case CLSongLine::LK_LINE:
				dc.SetFont(chordfont);
				dc.DrawText((*i)->GetChords(), 10, spos);
				spos+=dc.GetTextExtent((*i)->GetChords()).GetHeight();

				dc.SetFont(linefont);
				dc.DrawText((*i)->GetLine(), 10, spos);
				spos+=dc.GetTextExtent((*i)->GetLine()).GetHeight();
				break;
			case CLSongLine::LK_SPACE:
				dc.SetFont(linefont);
				spos+=dc.GetTextExtent(wxT("M")).GetHeight();
				break;
		}
	}
}
