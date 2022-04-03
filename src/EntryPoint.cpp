//These includes are specific to the way we’ve set up GLFW and GLAD.
#include "Program.h"

int main(void)
{
    Program program;
    program.Init(1280, 720, "App");
    program.Update();
    program.End();
    return 0;
}
