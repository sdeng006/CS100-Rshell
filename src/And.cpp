#include "Base.h"
#include "Connector.h"
#include "And.h"

// Constructors use the same one as Connector their parent's
And::And() : Connector() { }

And::And(Base* left, Base* right) : Connector(left, right) { }


// if the left side executes, then what this function returns depends on the right side (return either false or true)
// if the left side does not works, return false
bool And::execute() {
    if (left->execute()) {
        if (right->execute()) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}