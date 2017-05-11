#include "Base.h"
#include "Cmd.h"
#include "Exit.h"

Exit::Exit(): Cmd("Exit") {}

bool Exit::execute() {
    exit(0);
}