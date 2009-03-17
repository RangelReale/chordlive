#ifndef H__CLAPP__H
#define H__CLAPP__H

#include <iostream>

#include <wx/wx.h>

#include <SDL.h>
#include "clmain.h"

class CLApp : public wxApp {
    DECLARE_CLASS(CLApp)   
private:
    CLMain *frame;   
public:
    bool OnInit();
    int OnRun();
    int OnExit();
};

#endif // H__CLAPP__H