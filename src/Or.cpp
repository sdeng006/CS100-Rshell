#include "Base.h"
#include "Connector.h"
#include "Or.h"

// Constructors use the same one as Connector their parent's
Or::Or() : Connector() { }

Or::Or(Base* left, Base* right) : Connector(left, right) { }


// if the left side does not execute, then what this function returns depends on the right side (return either false or true)
// if the left side works, return false
bool Or::execute() {
    if (!left->execute()) {
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