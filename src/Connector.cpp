#include "Connector.h"
#include "Base.h"

// Constructors will apply same things into its children
Connector::Connector() {
    this->right = NULL;
    this->left = NULL;
}

Connector::Connector(Base* left, Base* right) {
    this->right = right;
    this->left = left;
}

// Set left/right will make independently changes in either left or right
void Connector::setRight(Base* right) {
    this->right = right;
}

void Connector::setLeft(Base* left) {
    this->left = left;
}