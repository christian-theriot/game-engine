#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <version.hpp>

int main(int argc, char **argv)
{
    Version version(1, 0, 0);
    std::cout << "Game Engine v" << version.get() << std::endl;
    return 0;
}