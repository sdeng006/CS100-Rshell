#include "Base.h"
#include "Connector.h"
#include "Semicolon.h"

// Constructors use the same one as Connector their parent's
Semicolon::Semicolon() : Connector() { }

Semicolon::Semicolon(Base* left) {
    this -> left = left; }


// the semi will execute it no matter left or right side return
// but running both is needed
bool Semicolon::execute() {
    left->execute();
    //right->execute();
    
    return true;
}