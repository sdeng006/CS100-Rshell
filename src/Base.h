#ifndef BASE_H
#define BASE_H

using namespace std;

// base for connector and cmd
class Base {
    public:
        virtual bool execute() = 0;
};

#endif