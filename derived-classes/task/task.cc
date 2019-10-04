#include "task.h"

namespace excpp {

void print_slowly(const std::string& message)
{
    int64_t time = 0;
    int64_t delay_between_characters = 100000;

    auto tasks = std::vector<std::unique_ptr<Task>>();

    for (auto c : message) {
        std::unique_ptr<Task> task = std::make_unique<Print>(c);
        task->delay(time);
        tasks.push_back(std::move(task));
        // More delay if it is a comma
        if (c == ',') {
            time += 16 * delay_between_characters;
        } else {
            time += delay_between_characters;
        }
    }

    auto scheduler = Scheduler();

    for (auto& task : tasks) {
        scheduler.add(*task);
    }

    for (size_t t = 0; t < time; ++t) {
        scheduler.run(1);
    }

}

} // namespace excpp

int main() {
    excpp::print_slowly("Hello, this is computer!\n");
    return 0;
}