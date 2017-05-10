#ifndef OR_H
#define OR_H

#include "Base.h"
#include "Connector.h"

class Or : public Connector {
    public:
        Or();
        Or(Base* left, Base* right);
        bool execute();
};

#endif