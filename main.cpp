#include <iostream>
#include <Infrastructure/tasks.hpp>

int main() {

    // Create a test task, this will run through the
    Task task("Kith Task", supported_sites::KITH, "/collections/footwear", {"Converse X"}, {"Flame"}, "9", 3, 3);
    task.run();

    return 0;
}