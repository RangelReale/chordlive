#include "clmain.h"

IMPLEMENT_CLASS(CLMain, wxFrame)

BEGIN_EVENT_TABLE(CLMain, wxFrame)
    EVT_MENU(IDM_FILE_EXIT, CLMain::onFileExit)
    EVT_MENU(IDM_HELP_ABOUT, CLMain::onHelpAbout)
END_EVENT_TABLE()

CLMain::CLMain() {
    // Create the CLMain
    Create(NULL, ID_FRAME, wxT("ChordLive"), wxDefaultPosition,
           wxDefaultSize, wxDEFAULT_FRAME_STYLE);

    // create the main menubar
    wxMenuBar *mb = new wxMenuBar;
    
    // create the file menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(IDM_FILE_EXIT, wxT("E&xit"));
    
    // add the file menu to the menu bar
    mb->Append(fileMenu, wxT("&File"));
    
    // create the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(IDM_HELP_ABOUT, wxT("About"));
    
    // add the help menu to the menu bar
    mb->Append(helpMenu, wxT("&Help"));
    
    // add the menu bar to the CLMain
    SetMenuBar(mb);

	song_ = new CLPanel(this);
}

void CLMain::onHelpAbout(wxCommandEvent &) {
    wxMessageBox(wxT("wx-sdl tutorial\nCopyright (C) 2005 John Ratliff"),
                 wxT("about wx-sdl tutorial"), wxOK | wxICON_INFORMATION);
}

void CLMain::onFileExit(wxCommandEvent &) {
	Close();
}
