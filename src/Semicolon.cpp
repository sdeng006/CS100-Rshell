#ifndef SEMICOLON_CPP
#define SEMICOLON_CPP

#include "Connector.h"
#include "Semicolon.h"

Semicolon::Semicolon(): Connector() {
    
}

Semicolon::Semicolon(Base* lhs, Base* rhs): Connector(lhs,rhs) {
    
}

void Semicolon::setLeft(Base* lhs) {
    this->lhs = lhs;
}

// set right
void Semicolon::setRight(Base* rhs) {
    this->rhs = rhs;
}

bool Semicolon::execute() {
    return true;
}

#endif