#ifndef H__CLAPP__H
#define H__CLAPP__H

#include <iostream>

#include <wx/wx.h>

#include "clmain.h"

class CLApp : public wxApp {
    DECLARE_CLASS(CLApp)   
private:
    CLMain *frame;   
public:
    virtual bool OnInit();
};

#endif // H__CLAPP__H