#ifndef H__CLMAIN__H
#define H__CLMAIN__H

#include <wx/wx.h>
#include "clpanel.h"

class CLMain : public wxFrame {
    DECLARE_CLASS(CLMain)
    DECLARE_EVENT_TABLE()
    
private:
    CLPanel *panel;

    void onFileExit(wxCommandEvent &event);
    void onHelpAbout(wxCommandEvent &event);   
public:
	enum {
		ID_FRAME = 10000,
		ID_PANEL,
		IDM_FILE_EXIT,
		IDM_HELP_ABOUT
	};

    CLMain();
    CLPanel &getPanel();
};

#endif // H__CLMAIN__H
