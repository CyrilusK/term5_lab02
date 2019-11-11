// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Experiments {
 private:
  const int loops = 1000;  // 1000 итераций
  unsigned int* items;  // массив рандомизированных чисел
  int* buf_sizes;       // размер буфера
  int size_exps;        // количество экспериментов
  struct Data {
    int id;
    string name_detour;
    int time;
  };
  vector<Data> exp;

 public:
  Experiments(int min, int max);
  int KbToSize(int kb);
  void InitItems(int length);
  void WarmingUp(int length);
  void straight_detour(int length);
  void reverse_detour(int length);
  void random_detour(int length);
  void action();
};
Experiments::Experiments(int min, int max) {
  size_exps = /*log2(max / min) + 3*/ 8;
  buf_sizes = new int[size_exps];
  int n = 0;
  min /= 2;  // Первый эксперимент
  while (min <= max * 3 / 2) {
    buf_sizes[n++] = min;
    min *= 2;
  }
  buf_sizes[n++] = max * 3 / 2;
  items = nullptr;
}

int Experiments::KbToSize(int kb) {  //Перевод в байт
  return 1024 * kb / sizeof(int);
}

void Experiments::InitItems(int length)  // Инициализация
{
  if (items != nullptr) delete[] items;
  items = new unsigned int[length];
  for (unsigned int i = 0; i < (unsigned int)length; i++) {
    items[i] = rand() % length;
  }
}

int get(int anything)  // Возвращение значения параметра для прогревания кэша
{
  return anything;
}

void Experiments::WarmingUp(int length) {  // Прогревание кэша
  int anything = 0;
  for (int i = 0; i < length; i++) {
    anything = items[i];
  }
  get(anything);
}

void Experiments::straight_detour(int length) {  // Прямый обход
  int anything = 0;
  for (int i = 0; i < loops; i++) {
    for (unsigned int j = 0; j < (unsigned int)length; j++) {
      anything = items[j];
    }
  }
  get(anything);
}

void Experiments::reverse_detour(int length) {  // Обратный обход
  int anything = 0;
  for (int i = 0; i < loops; i++) {
    for (int j = length - 1; j >= 0; j--) {
      anything = items[j];
    }
  }
  get(anything);
}

void Experiments::random_detour(int length) {  // Рандомный обход
  int anything = 0;
  for (int i = 0; i < loops; i++) {
    for (unsigned int j = 0; j < (unsigned int)length; j++) {
      anything = items[rand() % length];
    }
  }
  get(anything);
}

void Experiments::action() {
  ofstream out;
  out.open("data.txt");
  for (int i = 0; i < size_exps; i++) {
    int length = KbToSize(buf_sizes[i]);
    InitItems(length);
    WarmingUp(length);

    auto t_begin = chrono::high_resolution_clock::now();
    straight_detour(length);
    auto t_end = chrono::high_resolution_clock::now();
    auto t_dif =
        chrono::duration_cast<chrono::milliseconds>(t_end - t_begin).count();
    Data data{i + 1, "Straight", t_dif};
    exp.push_back(data);
    out << "investigation:\n\ttravel_variant: " << data.name_detour << endl;
    out << "\texperiments:\n\t- experiment:\n\t\tnumber: " << data.id << endl;
    out << "\t\tinput_data:\n\t\t\tbuffer_size: " << buf_sizes[i] << "mb\n";
    out << "\t\tresults:\n\t\t\tduration: " << data.time << "ms\n";

    t_begin = chrono::high_resolution_clock::now();
    reverse_detour(length);
    t_end = chrono::high_resolution_clock::now();
    t_dif =
        chrono::duration_cast<chrono::milliseconds>(t_end - t_begin).count();
    Data data1{i + 1, "Reverse", t_dif};
    exp.push_back(data1);
    out << "investigation:\n\ttravel_variant: " << data1.name_detour << endl;
    out << "\texperiments:\n\t- experiment:\n\t\tnumber: " << data1.id << endl;
    out << "\t\tinput_data:\n\t\t\tbuffer_size: " << buf_sizes[i] << "mb\n";
    out << "\t\tresults:\n\t\t\tduration: " << data1.time << "ms\n";

    t_begin = chrono::high_resolution_clock::now();
    random_detour(length);
    t_end = chrono::high_resolution_clock::now();
    t_dif =
        chrono::duration_cast<chrono::milliseconds>(t_end - t_begin).count();
    Data data2{i + 1, "Random", t_dif};
    exp.push_back(data2);
    out << "investigation:\n\ttravel_variant: " << data2.name_detour << endl;
    out << "\texperiments:\n\t- experiment:\n\t\tnumber: " << data2.id << endl;
    out << "\t\tinput_data:\n\t\t\tbuffer_size: " << buf_sizes[i] << "mb\n";
    out << "\t\tresults:\n\t\t\tduration: " << data2.time << "ms\n";
  }
}
#endif // INCLUDE_HEADER_HPP_
