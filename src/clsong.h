#ifndef H__CLSONG__H
#define H__CLSONG__H

#include <wx/wx.h>
#include <deque>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <wx/xml/xml.h>

// forward
class CLSongDraw;


/****
 *
 * CLSongException
 *
 ***/
class CLSongException : public std::exception {
public:
    CLSongException(const wxString &what) throw() : exception()
        { what_=what; }

    virtual ~CLSongException() throw () {}

    const wxString &what() { return what_; }
private:
    wxString what_;
};


/****
 *
 * CLSongLine
 *
 ***/
class CLSongLine
{
public:
	enum linekind_t {
		LK_NONE,
		LK_LINE,
		LK_SPACE,
	};

	CLSongLine();
	CLSongLine(linekind_t linekind, const wxString &line = wxEmptyString, const wxString &chords = wxEmptyString);
	virtual ~CLSongLine();

	static bool IsChord(const wxString &chars);
	static bool IsChordLine(const wxString &line);

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

	const wxString &GetTitle() { return title_; }
	void SetTitle(const wxString &title) { title_=title; }
	const wxString &GetArtist() { return artist_; }
	void SetArtist(const wxString &artist) { artist_=artist; }

	void Clear();
	void Parse(const wxString &text);

	void LoadFromFile(const wxString &filename);
	void SaveToFile(const wxString &filename);

	void LoadFromStream(wxInputStream &stream);
	void SaveToStream(wxOutputStream &stream);
protected:
	void LoadLines(wxXmlNode *lines);
	void LoadLine(wxXmlNode *line);
private:
	friend class CLSongDraw;
	
	typedef boost::shared_ptr<CLSongLine> songlistitem_t;
	typedef std::deque< songlistitem_t > songlist_t;

	wxString title_, artist_;
	songlist_t lines_;
};

/****
 *
 * CLSongDraw
 *
 ***/
class CLSongDraw
{
public:
	CLSongDraw(CLSong *song);
	virtual ~CLSongDraw();

	int GetStartPos() { return startpos_; }
	void SetStartPos(int startpos) { startpos_=startpos; }

	void Draw(wxDC &dc, const wxRect &rect);
private:
	CLSong *song_;
	int startpos_;
};

#endif // H__CLSONG__H
