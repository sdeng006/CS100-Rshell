#ifndef SEMICOLON_H
#define SEMICOLON_H

#include "Base.h"
#include "Connector.h"

class Semicolon : public Connector {
    public:
        Semicolon();
        Semicolon(Base* left, Base* right);
        bool execute();
};

#endif