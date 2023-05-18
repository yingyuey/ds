#include <iostream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(arr[minIndex], arr[i]);
    }
}
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[m-l+1], R[r-m];
    for (int i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l >= r) {
        return;
    }
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void testSort(vector<int>& arr) {
    // 六种排序算法分别进行测试
    auto start = chrono::high_resolution_clock::now();
    bubbleSort(arr);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_castchrono::microseconds(end - start);
    cout << “BubbleSort: " << duration.count() << " microseconds” << endl;

    start = chrono::high_resolution_clock::now();
    insertionSort(arr);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "InsertionSort: " << duration.count() << " microseconds" << endl;

    start = chrono::high_resolution_clock::now();
    selectionSort(arr);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "SelectionSort: " << duration.count() << " microseconds" << endl;

    start = chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "MergeSort: " << duration.count() << " microseconds" << endl;

    start = chrono::high_resolution_clock::now();
    quickSort(arr, 0, arr.size() - 1);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "QuickSort: " << duration.count() << " microseconds" << endl;

    start = chrono::high_resolution_clock::now();
    heapSort(arr);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "HeapSort: " << duration.count() << " microseconds" << endl;
}
vector<int> generateInOrder(int n) {
    vector<int> arr;
    for (int i = 1; i <= n; i++) {
        arr.push_back(i);
    }
    return arr;
}
// 逆序序列生成函数
vector<int> generateInReverse(int n) {
    vector<int> arr;
    for (int i = n; i > 0; i--) {
        arr.push_back(i);
    }
    return arr;
}
// 随机序列生成函数
vector<int> generateRandom(int n) {
    vector<int> arr;
    default_random_engine generator;
    uniform_int_distribution<int> distribution(1, n);
    for (int i = 1; i <= n; i++) {
        arr.push_back(distribution(generator));
    }
    return arr;
}
int main() {
    int n = 10000;

    // 生成顺序序列
    vector<int> arr = generateInOrder(n);
    cout << "In Order Data:" << endl;
    testSort(arr);
    cout << endl;

    // 生成逆序序列
    arr = generateInReverse(n);
    cout << "In Reverse Order Data:" << endl;
    testSort(arr);
    cout << endl;

    // 生成随机序列
    arr = generateRandom(n);
    cout << "In Random Order Data:" << endl;
    testSort(arr);

    return 0;
}