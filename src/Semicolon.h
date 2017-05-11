#ifndef SEMICOLON_H
#define SEMICOLON_H

#include "Connector.h"

class Semicolon: public Connector {
    public:
        Semicolon();
        Semicolon(Base* lhs, Base* rhs);
void setLeft(Base* lhs);
        void setRight(Base* rhs);
        bool execute();
};

#endif