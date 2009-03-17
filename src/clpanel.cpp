#include "clpanel.h"
#include "clmain.h"

inline void CLPanel::onEraseBackground(wxEraseEvent &) {}

IMPLEMENT_CLASS(CLPanel, wxPanel)

BEGIN_EVENT_TABLE(CLPanel, wxPanel)
    EVT_PAINT(CLPanel::onPaint)
    EVT_ERASE_BACKGROUND(CLPanel::onEraseBackground)
    EVT_IDLE(CLPanel::onIdle)
END_EVENT_TABLE()

CLPanel::CLPanel(wxWindow *parent) : wxPanel(parent, CLMain::ID_PANEL), screen(NULL) {
    // ensure the size of the wxPanel
    wxSize size(640, 480);
    
    SetMinSize(size);
    SetMaxSize(size);
}

CLPanel::~CLPanel() {
    if (screen != NULL) {
        SDL_FreeSurface(screen);
    }
}

void CLPanel::onPaint(wxPaintEvent &) {
    // can't draw if the screen doesn't exist yet
    if (screen == NULL) {
        return;
    }
    
    // lock the surface if necessary
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }
    
    // create a bitmap from our pixel data
    wxBitmap bmp(wxImage(screen->w, screen->h, 
                    static_cast<unsigned char *>(screen->pixels), true));
    
    // unlock the screen
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
    
    // paint the screen
    wxBufferedPaintDC dc(this, bmp);
}

void CLPanel::onIdle(wxIdleEvent &) {
    // create the SDL_Surface
    createScreen();
    
    // Lock surface if needed
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }
    
    // Ask SDL for the time in milliseconds
    int tick = SDL_GetTicks();
    
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            wxUint32 color = (y * y) + (x * x) + tick;
            wxUint8 *pixels = static_cast<wxUint8 *>(screen->pixels) + 
                              (y * screen->pitch) +
                              (x * screen->format->BytesPerPixel);

            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixels[0] = color & 0xFF;
                pixels[1] = (color >> 8) & 0xFF;
                pixels[2] = (color >> 16) & 0xFF;
            #else
                pixels[0] = (color >> 16) & 0xFF;
                pixels[1] = (color >> 8) & 0xFF;
                pixels[2] = color & 0xFF;
            #endif
        }
    }
    
    // Unlock if needed
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
    
    // refresh the panel
    Refresh(false);
    
    // throttle to keep from flooding the event queue
    wxMilliSleep(33);
}

void CLPanel::createScreen() {
    if (screen == NULL) {
        int width, height;
        GetSize(&width, &height);
        
        screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 
                                      24, 0, 0, 0, 0);     
    }
}
