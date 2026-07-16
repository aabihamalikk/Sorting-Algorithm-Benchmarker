#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
using namespace chrono;


void generateRandomFile(const char* filename, int n) {
    ofstream fout(filename);
    for (int i = 0; i < n; i++)
        fout << rand() % 100000 << " ";
    fout.close();
}

void generateReverseFile(const char* filename, int n) {
    ofstream fout(filename);
    for (int i = n; i > 0; i--)
        fout << i << " ";
    fout.close();
}

void readFile(const char* filename, int* arr, int n) {
    ifstream fin(filename);
    for (int i = 0; i < n; i++)
        fin >> arr[i];
    fin.close();
}

void copyArray(int* src, int* dest, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}


void bubbleSort(int* arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

void selectionSort(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIdx])
                minIdx = j;
        swap(arr[i], arr[minIdx]);
    }
}

void insertionSort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}


void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int partitionQS(int* arr, int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr[randomIndex], arr[high]);   

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partitionQS(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void heapify(int* arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}


int main() {
    srand(time(0));

    int sizes[] = { 1000, 2000, 3000, 4000, 5000, 10000, 20000, 40000, 80000, 160000, 250000, 500000 };

    ofstream csv("performance.csv");
    csv << "Case,Size,Algorithm,Time(ms)\n";

    for (int n : sizes) {
        int* original = new int[n];
        int* arr = new int[n];

        const char* cases[] = { "Random", "Reverse" };

        for (int c = 0; c < 2; c++) {
            if (c == 0) generateRandomFile("input.txt", n);
            else generateReverseFile("input.txt", n);

            readFile("input.txt", original, n);

            cout << "Running Size: " << n << " | Case: " << cases[c] << "..." << endl;

            if (n <= 160000) {
                copyArray(original, arr, n);
                cout << "  - Bubble Sort... " << flush;
                auto st = high_resolution_clock::now();
                bubbleSort(arr, n);
                auto en = high_resolution_clock::now();
                cout << "Done." << endl;
                csv << cases[c] << "," << n << ",Bubble,"
                    << duration_cast<milliseconds>(en - st).count() << "\n";
            }

            if (n <= 160000) {
                copyArray(original, arr, n);
                cout << "  - Insertion Sort... " << flush;
                auto st = high_resolution_clock::now();
                insertionSort(arr, n);
                auto en = high_resolution_clock::now();
                cout << "Done." << endl;
                csv << cases[c] << "," << n << ",Insertion,"
                    << duration_cast<milliseconds>(en - st).count() << "\n";
            }

            if (n <= 160000) {
                copyArray(original, arr, n);
                cout << "  - Selection Sort... " << flush;
                auto st = high_resolution_clock::now();
                selectionSort(arr, n);
                auto en = high_resolution_clock::now();
                cout << "Done." << endl;
                csv << cases[c] << "," << n << ",Selection,"
                    << duration_cast<milliseconds>(en - st).count() << "\n";
            }

            copyArray(original, arr, n);
            cout << "  - Merge Sort... " << flush;
            auto st = high_resolution_clock::now();
            mergeSort(arr, 0, n - 1);
            auto en = high_resolution_clock::now();
            cout << "Done." << endl;
            csv << cases[c] << "," << n << ",Merge,"
                << duration_cast<milliseconds>(en - st).count() << "\n";

            copyArray(original, arr, n);
            cout << "  - Quick Sort... " << flush;
            st = high_resolution_clock::now();
            quickSort(arr, 0, n - 1);
            en = high_resolution_clock::now();
            cout << "Done." << endl;
            csv << cases[c] << "," << n << ",Quick,"
                << duration_cast<milliseconds>(en - st).count() << "\n";

            copyArray(original, arr, n);
            cout << "  - Heap Sort... " << flush;
            st = high_resolution_clock::now();
            heapSort(arr, n);
            en = high_resolution_clock::now();
            cout << "Done." << endl;
            csv << cases[c] << "," << n << ",Heap,"
                << duration_cast<milliseconds>(en - st).count() << "\n";
        }

        delete[] original;
        delete[] arr;
    }

    csv.close();
    cout << "Execution completed successfully.\n";
    return 0;
}
