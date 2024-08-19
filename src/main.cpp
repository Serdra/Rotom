#include "includes.h"

int main() {
    srand(time(0));
    std::string input;
    std::cin >> input;

    if (input == "ugi") UGI();
}