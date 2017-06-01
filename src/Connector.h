#ifndef CONNECTOR_H  
#define CONNECTOR_H

#include "Base.h"

class Connector : public Base {
    // for its children (AND, OR, and SEMI) to use as protected
    protected:
        Base* left;
        Base* right;
    
    public:
        Connector();
        Connector(Base* left, Base* right);
        // seemly below ones useless:
        // void setLeft(Base* left);
        // void setRight(Base* right);
        virtual bool execute() = 0;
};

class And : public Connector {
    public:
        And(Base* left, Base* right);
        bool execute();
};

class Or : public Connector {
    public:
        Or(Base* left, Base* right);
        bool execute();
};

class Semicolon : public Connector {
    public:
        Semicolon(Base* left);
        Semicolon(Base* left, Base* right);
        bool execute();
};

#endif
