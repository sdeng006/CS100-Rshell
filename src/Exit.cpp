#include "Base.h"
#include "Cmd.h"
#include "Exit.h"

Exit::Exit(): Cmd("Exit") {}

bool Eixt::executre() {
    exit(0);
}