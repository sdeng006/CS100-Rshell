#include "Base.h"
#include "Cmd.h"
#include "Exit.h"

Exit::Exit(): Cmd("Exit") {}

bool Exit::executre() {
    exit(0);
}