#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <Windows.h>

std::mutex m;

void moveCursorTo(int x, int y) {
   std::cout << "\033[" << y << ";" << x << "H";
}

void calculate(int thread_number, int work_length, int row) {
   auto start_time = std::chrono::high_resolution_clock::now();

   for (int i = 0; i <= work_length; ++i) {

      {
         std::lock_guard<std::mutex> lock(m);
         moveCursorTo(0, row);
         std::cout << std::setw(2) << thread_number << "  "
            << std::setw(8) << std::left << std::this_thread::get_id() << "    "; // создание шапки

         for (int j = 0; j < i; ++j) {
            std::cout << "=";

            std::this_thread::sleep_for(std::chrono::milliseconds(3));
         }

      }
   }

   {
      std::lock_guard<std::mutex> lock(m);
      auto end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> total_end = end - start_time;

      std::string t_e = std::to_string(total_end.count());

      moveCursorTo(48, row);       // установка курсора для вывода времени выполнения потока
      std::cout << t_e << " сек.";
   }
}

int main() {
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);
   int num_threads = 4; 
   int work_length = 30;

   std::vector<std::thread> threads;

   std::cout << "#  "<<std::setw(4)  << "id" << std::setw(15) << "\t\tProgress Bar" << std::setw(17)<< "Time\n\n";
   
   for (int i = 0; i < num_threads; ++i) {
      threads.emplace_back(calculate, i + 1, work_length, i + 2);
   }

   for (auto& thread : threads) {
      thread.join();
   }

   std::cout << "\n\n\n\n";

   return EXIT_SUCCESS;
}