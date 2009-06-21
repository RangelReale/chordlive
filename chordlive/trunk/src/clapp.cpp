#include "clapp.h"

bool CLApp::OnInit() {
    // create the CLMain
    frame = new CLMain;
    frame->SetClientSize(640, 480);
    frame->Centre();
    frame->Show();
    
    // Our CLMain is the Top Window
    SetTopWindow(frame);

    // initialization should always succeed
    return true;
}

IMPLEMENT_CLASS(CLApp, wxApp)
IMPLEMENT_APP(CLApp)
