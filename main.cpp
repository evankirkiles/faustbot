#include <iostream>
#include <Infrastructure/tasks.hpp>

int main() {

    // Create a test task and run it
    Task task("Bodega Task", supported_sites::BODEGA, "1", "/collections/footwear", {"Air Max"}, {"Cargo"}, "9", 3, 3);
    task.run();

    return 0;
}