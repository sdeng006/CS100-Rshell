#include "Connector.h"

// Constructors use the same one as Connector their parent's
Connector::Connector() {} 
Connector::Connector(Base* left, Base* right) {
    this->left = left;
    this->right = right;
}

// and case
And::And(Base* left, Base* right) {
    this->left = left;
    this->right = right;
}
bool And::execute() {
    // if the left side executes, then what this function returns depends on the right side (return either false or true)
    // if the left side does not works, return false
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

// or case
Or::Or(Base* left, Base* right) {
    this->left = left;
    this->right = right;
}
bool Or::execute() {
    // if the left side does not execute, then what this function returns depends on the right side (return either false or true)
    // if the left side works, return false
    if (!left->execute()) {
        if (right->execute()) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
}

// semi case
Semicolon::Semicolon(Base* left, Base* right) {
    this->left = left;
    this->right = right;
}
Semicolon::Semicolon(Base* left) {
    this->left = left;
    this->right = NULL;
}
bool Semicolon::execute() {
    // the semi will execute it no matter left or right side return
    // but running both is needed
    left->execute();
    
    if (right != NULL) {
        if (right->execute()) {
            return true;
        }
        else {
            return false;
        }
    }
    
    return true;
    
}