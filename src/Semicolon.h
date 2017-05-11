#ifndef SEMICOLON_H
#define SEMICOLON_H

#include "Base.h"
#include "Connector.h"

class Semicolon : public Connector {
    public:
        Semicolon();
        Semicolon(Base* left);
        bool execute();
};

#endif