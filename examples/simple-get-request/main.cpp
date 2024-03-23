#include <crapper/crapper.hpp>
#include <iostream>

int main() {
    auto crapper = Crapper{};
    auto const buffer = crapper.get("https://official-joke-api.appspot.com/random_joke");

    std::cout << "num bytes received: " << buffer.length() << '\n';
    std::cout << buffer << '\n';
}
