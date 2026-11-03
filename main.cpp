#include <windows.h>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

const int N = 10000;

struct Statistics {
    uint64_t comparisons = 0;
    uint64_t writes = 0;
};

void bubbleSort(vector<int>& a, Statistics& stats) {
    int n = static_cast<int>(a.size());

    for (int i = 0; i < n - 1; i++) {
        bool changed = false;

        for (int j = 0; j < n - 1 - i; j++) {
            stats.comparisons++;

            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;

                stats.writes += 2;
                changed = true;
            }
        }

        if (!changed) {
            break;
        }
    }
}

void insertionSort(vector<int>& a, Statistics& stats) {
    int n = static_cast<int>(a.size());

    for (int i = 1; i < n; i++) {
        int value = a[i];
        int j = i - 1;

        while (j >= 0) {
            stats.comparisons++;

            if (a[j] > value) {
                a[j + 1] = a[j];
                stats.writes++;
                j--;
            }
            else {
                break;
            }
        }

        a[j + 1] = value;
        stats.writes++;
    }
}

int hoarePartition(vector<int>& a, int left, int right, Statistics& stats) {
    int pivot = a[left + (right - left) / 2];
    int i = left - 1;
    int j = right + 1;

    while (true) {
        do {
            i++;
            stats.comparisons++;
        } while (a[i] < pivot);

        do {
            j--;
            stats.comparisons++;
        } while (a[j] > pivot);

        if (i >= j) {
            return j;
        }

        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;

        stats.writes += 2;
    }
}

void quickSort(vector<int>& a, int left, int right, Statistics& stats) {
    if (left >= right) {
        return;
    }

    int split = hoarePartition(a, left, right, stats);

    quickSort(a, left, split, stats);
    quickSort(a, split + 1, right, stats);
}

void printStatistics(const char* name, const Statistics& stats) {
    cout << name << ":\n";
    cout << "  Сравнений элементов: " << stats.comparisons << '\n';
    cout << "  Записей в массив: " << stats.writes << '\n';
    cout << "  Всего операций: "
        << stats.comparisons + stats.writes << "\n\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    vector<int> original(N);

    mt19937 generator(42);
    uniform_int_distribution<int> distribution(-100, 100);

    for (int& number : original) {
        number = distribution(generator);
    }

    vector<int> bubbleArray = original;
    vector<int> insertionArray = original;
    vector<int> quickArray = original;

    Statistics bubbleStats;
    Statistics insertionStats;
    Statistics quickStats;

    bubbleSort(bubbleArray, bubbleStats);
    insertionSort(insertionArray, insertionStats);
    quickSort(quickArray, 0, N - 1, quickStats);

    cout << "Размер массива: " << N << " элементов\n";
    cout << "Диапазон чисел: от -100 до 100\n\n";

    printStatistics("Пузырьковая сортировка", bubbleStats);
    printStatistics("Сортировка вставками", insertionStats);
    printStatistics("Быстрая сортировка Хоара", quickStats);

    return 0;
}