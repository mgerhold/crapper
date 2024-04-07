#include <crapper/crapper.hpp>
#include <iostream>

int main() {
    static constexpr auto url = "https://official-joke-api.appspot.com/random_joke";

    auto const response = Crapper{}.get(url).send();

    std::cout << "num bytes received: " << response.length() << '\n';
    std::cout << response << '\n';

    auto crapper = Crapper{};
    std::cout << crapper.get(url).send() << '\n';
}
