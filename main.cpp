#include <iostream>
#include <Infrastructure/tasks.hpp>

int main() {

    // Create a test task and run it
    Task task("Kith Task", supported_sites::KITH, "1", "/collections/footwear", {"Converse X"}, {"Flame"}, "9", 3, 3);
    task.run();

    return 0;
}