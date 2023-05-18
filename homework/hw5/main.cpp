#include"../../vector.xxx/vector.h"
#include<iostream>
#include <random>
#include <chrono>

// 交换两个元素
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
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
// 逆序序列生成函数
vector<int> generateInReverse(int n) {
    vector<int> arr;
    for (int i = n; i > 0; i--) {
        arr.push_back(i);
    }
    return arr;
}
// 顺序序列生成函数
vector<int> generateInOrder(int n) {
    vector<int> arr;
    for (int i = 1; i <= n; i++) {
        arr.push_back(i);
    }
    return arr;
}
// 起泡排序算法
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

// 插入排序算法
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int j = i;
        while (j > 0 && arr[j - 1] > arr[j]) {
            swap(arr[j - 1], arr[j]);
            j--;
        }
    }
}

// 选择排序算法
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}

// 归并排序算法
void merge(vector<int>& arr, int left, int right, int mid) {
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);
    int n1 = leftArr.size();
    int n2 = rightArr.size();
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] < rightArr[j]) {
            arr[k++] = leftArr[i++];
        }
        else {
            arr[k++] = rightArr[j++];
        }
    }
    while (i < n1) {
        arr[k++] = leftArr[i++];
    }
    while (j < n2) {
        arr[k++] = rightArr[j++];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, right, mid);
    }
}

// 快速排序算法
int partition(vector<int>& arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1;
}

void quickSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int pivot = partition(arr, left, right);
        quickSort(arr, left, pivot - 1);
        quickSort(arr, pivot + 1, right);
    }
}
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
void testSort(vector<int>& arr)
{
    // 六种排序算法分别进行测试
    auto start = chrono::high_resolution_clock::now();
    bubbleSort(arr);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "BubbleSort: " << duration.count() << " microseconds" << endl;

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