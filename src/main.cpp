#include "window.hpp"
#include <cstdlib>
#include <ctime>

constexpr int WIDTH{480};
constexpr int HEIGHT{630};

int main() {
    std::srand(std::time(0));
    Window window(WIDTH, HEIGHT, "Tetris");
    window.loop();
    return 0;
}
