#include <vector>
#include <string>
#include <iostream>

template <typename T>
void move_vectors(std::vector<T>& from, std::vector<T>& to) {
    to = std::move(from);
    from.clear(); // Опционально: гарантируем, что исходный вектор пуст
}

//
int main()
{
    std::vector<std::string> one = { "test_string1", "test_string2" };
    std::vector<std::string> two;

    move_vectors(one, two);

    // Проверка результата
    std::cout << "one size: " << one.size() << std::endl;
    std::cout << "two size: " << two.size() << std::endl;

    for (const auto& str : two) {
        std::cout << str << std::endl;
    }

    return 0;
}