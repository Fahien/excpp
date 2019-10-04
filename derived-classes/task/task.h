#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace excpp
{
class Scheduler;

/// Specific tasks can be objects of classes derived from this.
/// The program could be specified as a virtual function.
/// We can pass arguments as arguments to its constructors.
class Task
{
  public:
    void delay(int64_t time)
    {
        delay_time += time;
    }

    /// @return Whether delay has expired
    bool update(int64_t time)
    {
        delay_time -= time;
        return delay_time <= 0;
    }

    virtual void operator()() = 0;

  private:
    int64_t delay_time = 0;
};

class Print : public Task
{
  public:
    Print(char c) : c{c} {}

    void operator()() override
    {
        std::printf("%c", c);
    }

  private:
    char c;
};

class Scheduler
{
  public:
    void add(Task &t)
    {
        tasks.push_back(&t);
    }

    void run(int64_t time)
    {
        std::vector<std::vector<Task*>::iterator> to_remove;

        for (auto it = std::begin(tasks); it != std::end(tasks); ++it)
        {
            auto& task = *it;

            if (task->update(time)) {
                (*task)();
                to_remove.emplace_back(it);
            }
        }

        for(auto& it : to_remove) {
            tasks.erase(it);
        }
    }

  private:
    std::vector<Task *> tasks;
};

} // namespace excpp
