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
        void setLeft(Base* left);
        void setRight(Base* right);
        virtual bool execute() = 0;
};

#endif