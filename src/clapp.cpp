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

int CLApp::OnRun() {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO
#ifdef _DEBUG
		|SDL_INIT_NOPARACHUTE
#endif
		) < 0) {
        std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
        
        return -1;
    }
    
    // generate an initial idle event to start things
    wxIdleEvent event;
    event.SetEventObject(&frame->getPanel());
    frame->getPanel().AddPendingEvent(event);

    // start the main loop
    return wxApp::OnRun();
}

int CLApp::OnExit() {
    // cleanup SDL
    SDL_Quit();
    
    // return the standard exit code
    return wxApp::OnExit();
}

IMPLEMENT_CLASS(CLApp, wxApp)
IMPLEMENT_APP(CLApp)
