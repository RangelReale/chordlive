#include "clsong.h"
#include <wx/wfstream.h>
#include <wx/tokenzr.h>

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

bool CLSongLine::IsChord(const wxString &chars)
{
	if (chars.Left(1)!=wxT("C") &&
		chars.Left(1)!=wxT("D") &&
		chars.Left(1)!=wxT("E") &&
		chars.Left(1)!=wxT("F") &&
		chars.Left(1)!=wxT("G") &&
		chars.Left(1)!=wxT("A") &&
		chars.Left(1)!=wxT("B"))
		return false;

	return true;
}

bool CLSongLine::IsChordLine(const wxString &line)
{
	bool ret=true;
	// detect chord line
	wxStringTokenizer chstr(line, wxT(" "), wxTOKEN_DEFAULT);
	while ( chstr.HasMoreTokens() ) {
		wxString chtoken = chstr.GetNextToken();
		
		if (!CLSongLine::IsChord(chtoken)) {
			ret=false;
			break;
		}
	}
	return ret;
}


/****
 *
 * CLSong
 *
 ***/
CLSong::CLSong()
{
/*
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
*/
}

CLSong::~CLSong()
{

}

void CLSong::Clear()
{
	title_=wxT("");
	artist_=wxT("");
	lines_.clear();
}

void CLSong::Parse(const wxString &text)
{
	wxString mtext(text);
	mtext.Replace(wxT("\t"), wxT("    "));

	CLSongLine::linekind_t n_linekind = CLSongLine::LK_NONE;
	wxString n_lyrics(wxT("")), n_chords(wxT(""));

	lines_.clear();

	wxStringTokenizer str(mtext, wxT("\n"), wxTOKEN_RET_EMPTY);
	while ( str.HasMoreTokens() )
	{
		wxString token = str.GetNextToken();

		n_linekind=CLSongLine::LK_NONE;
		// white line
		if (token.IsEmpty())
		{
			if (!n_chords.IsEmpty())
				lines_.push_back(songlistitem_t(new CLSongLine(CLSongLine::LK_LINE, wxT(""), n_chords)));

			n_linekind=CLSongLine::LK_SPACE;
			n_lyrics=wxT("");
			n_chords=wxT("");

			lines_.push_back(songlistitem_t(new CLSongLine(n_linekind, n_lyrics, n_chords)));
		} else {
			if (CLSongLine::IsChordLine(token)) {
				n_chords=token.Trim(false);
			} else {
				n_linekind=CLSongLine::LK_LINE;
				n_lyrics=token.Trim(false);
				lines_.push_back(songlistitem_t(new CLSongLine(n_linekind, n_lyrics, n_chords)));

				n_lyrics=n_chords=wxT("");
			}
		}
	}

}


void CLSong::Draw(wxDC &dc, const wxRect &rect, int startpos)
{
	wxFont titlefont(13, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont linefont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont chordfont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	int spos=-1*startpos;
	dc.SetFont(titlefont);
	dc.DrawText(title_, 10, spos);
	spos+=dc.GetTextExtent(title_).GetHeight();

	dc.DrawText(artist_, 10, spos);
	spos+=dc.GetTextExtent(artist_).GetHeight();

	spos+=dc.GetTextExtent(wxT("M")).GetHeight();

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

void CLSong::LoadFromStream(wxInputStream &stream)
{
	wxXmlDocument doc;
	if (!doc.Load(stream))
		throw new CLSongException(_("Invalid ChordLive song data"));
	if (doc.GetRoot()->GetName() != wxT("chordlivesong"))
		throw new CLSongException(_("Invalid ChordLive song data"));

	Clear();

	wxXmlNode *child = doc.GetRoot()->GetChildren();
	while (child) {

		if (child->GetName() == wxT("title")) {
			title_=child->GetNodeContent();
		} else if (child->GetName() == wxT("artist")) {
			artist_=child->GetNodeContent();
		} else if (child->GetName() == wxT("keywords")) {
			// TODO
		} else if (child->GetName() == wxT("lines")) {
			// load lines
			LoadLines(child);
		}
		child = child->GetNext();
	}

}

void CLSong::LoadLines(wxXmlNode *lines)
{
	wxXmlNode *child = lines->GetChildren();
	while (child) {
		if (child->GetName() == wxT("line")) {
			LoadLine(child);
		}
		child = child->GetNext();
	}
}

void CLSong::LoadLine(wxXmlNode *line)
{
	CLSongLine::linekind_t n_linekind = CLSongLine::LK_LINE;
	wxString n_lyrics(wxT("")), n_chords(wxT(""));

	n_linekind=line->GetPropVal(wxT("kind"), wxT("line"))==wxT("space")?CLSongLine::LK_SPACE:CLSongLine::LK_LINE;

	wxXmlNode *child = line->GetChildren();
	while (child) {
		if (child->GetName() == wxT("lyrics")) {
			n_lyrics=child->GetNodeContent();
		} else if (child->GetName() == wxT("chords")) {
			n_chords=child->GetNodeContent();
		}
		child = child->GetNext();
	}

	if (n_linekind == CLSongLine::LK_SPACE || !n_lyrics.IsEmpty() || !n_chords.IsEmpty())
		lines_.push_back(songlistitem_t(new CLSongLine(n_linekind, n_lyrics, n_chords)));
}


void CLSong::SaveToStream(wxOutputStream &stream)
{

}

void CLSong::LoadFromFile(const wxString &filename)
{
	wxFileInputStream f(filename);
	LoadFromStream(f);
}

void CLSong::SaveToFile(const wxString &filename)
{
	wxFileOutputStream f(filename);
	SaveToStream(f);
}
