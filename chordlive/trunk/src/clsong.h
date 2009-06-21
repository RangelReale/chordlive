#ifndef H__CLSONG__H
#define H__CLSONG__H

#include <wx/wx.h>
#include <deque>
#include <boost/shared_ptr.hpp>

/****
 *
 * CLSongLine
 *
 ***/
class CLSongLine
{
public:
	enum linekind_t {
		LK_LINE,
		LK_SPACE,
	};

	CLSongLine();
	CLSongLine(linekind_t linekind, const wxString &line = wxEmptyString, const wxString &chords = wxEmptyString);
	virtual ~CLSongLine();

	linekind_t GetLineKind() { return linekind_; }
	const wxString &GetLine() { return line_; }
	const wxString &GetChords() { return chords_; }
private:
	linekind_t linekind_;
	wxString chords_, line_;
};

/****
 *
 * CLSong
 *
 ***/
class CLSong
{
public:
	CLSong();
	virtual ~CLSong();

	void Draw(wxDC &dc, const wxRect &rect);
private:
	typedef boost::shared_ptr<CLSongLine> songlistitem_t;
	typedef std::deque< songlistitem_t > songlist_t;

	wxString title_, artist_;
	songlist_t lines_;
};

#endif // H__CLSONG__H
