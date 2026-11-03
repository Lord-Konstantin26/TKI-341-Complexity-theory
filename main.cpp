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
        int left = 0;
        int right = i;

        while (left < right) {
            int middle = left + (right - left) / 2;
            stats.comparisons++;

            if (a[middle] <= value) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }

        for (int j = i; j > left; j--) {
            a[j] = a[j - 1];
            stats.writes++;
        }

        a[left] = value;
        stats.writes++;
    }
}

void mergeSort(vector<int>& a, Statistics& stats) {
    if (a.size() <= 1) {
        return;
    }

    int pivotIndex = static_cast<int>(a.size()) / 2;
    int pivot = a[pivotIndex];

    vector<int> left;
    vector<int> right;

    for (int i = 0; i < static_cast<int>(a.size()); i++) {
        if (i == pivotIndex) {
            continue;
        }

        stats.comparisons++;

        if (a[i] <= pivot) {
            left.push_back(a[i]);
            stats.writes++;
        } else {
            right.push_back(a[i]);
            stats.writes++;
        }
    }

    mergeSort(left, stats);
    mergeSort(right, stats);

    int index = 0;

    for (int value : left) {
        a[index] = value;
        stats.writes++;
        index++;
    }

    a[index] = pivot;
    stats.writes++;
    index++;

    for (int value : right) {
        a[index] = value;
        stats.writes++;
        index++;
    }
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
    vector<int> mergeArray = original;

    Statistics bubbleStats;
    Statistics insertionStats;
    Statistics mergeStats;

    bubbleSort(bubbleArray, bubbleStats);
    insertionSort(insertionArray, insertionStats);
    mergeSort(mergeArray, mergeStats);

    cout << "Размер массива: " << N << " элементов\n";
    cout << "Диапазон чисел: от -100 до 100\n\n";

    printStatistics("Пузырьковая сортировка", bubbleStats);
    printStatistics("Сортировка вставками с бинарным поиском", insertionStats);
    printStatistics("Третья сортировка", mergeStats);

    return 0;
}
