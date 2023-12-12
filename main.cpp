#ifndef _DEBUG
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif


#include ".\Main\app.h"

int main() {
    
    GUI();

    return 0;
}



