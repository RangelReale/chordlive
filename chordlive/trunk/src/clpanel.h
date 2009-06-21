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

    void onPaint(wxPaintEvent &event);
    void onEraseBackground(wxEraseEvent &event);
public:
    CLPanel(wxWindow *parent);
    ~CLPanel();
};



#endif // H__CLPANEL__H
