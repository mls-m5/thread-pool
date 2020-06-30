//! Author: Mattias Larson Sköld 2020
//! Run applications from stdin in paralell
//! *************************************************************************

#include <atomic>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace {

std::mutex taskAssignMutex;

bool error = false;

} // namespace

std::optional<std::string> getTask()
{
   std::lock_guard guard(taskAssignMutex);

   if (error)
   {
      return {};
   }

   if (std::string line; getline(std::cin, line))
   {
      return {line};
   }
   return {};
}

const std::string helpString = R"_(
      thread_pool

      runs commands specified with multiple threads

      flags:
      -h           print this text


      example:
      echo "echo hello" | thread_pool
)_";

int main(int /*argc*/, char* /*argv*/ [])
{
   std::vector<std::thread> threads;

   std::atomic_ulong numJobs = 0;

   const size_t numThreads = std::thread::hardware_concurrency();

   std::cout << "running with " << numThreads << " threads\n";

   threads.reserve(numThreads);

   for (size_t i = 0; i < numThreads; ++i)
   {
      threads.emplace_back([&numJobs]() {
         while (auto command = getTask())
         {
            if (system(command->c_str()))
            {
               error = true;

               std::cerr << "command '" << *command << "' failed" << std::endl;
            }
            else
            {
               ++numJobs;
            }
         }
      });
   }

   for (auto& thread : threads)
   {
      thread.join();
   }

   std::cout << " done running " << numJobs << std::endl;

   return error;
}

