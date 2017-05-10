#ifndef AND_H
#define AND_H

#include "Base.h"
#include "Connector.h"

class And : public Connector {
    public:
        And();
        And(Base* left, Base* right);
        bool execute();
};

#endif