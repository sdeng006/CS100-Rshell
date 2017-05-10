#ifndef EXIT_H
#define EXIT_H

#include "Base.h"
#include "Cmd.h"

class Exit: public Cmd {
    public:
        Exit();
        
        bool execute();
};

#endif