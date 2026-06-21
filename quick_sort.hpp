#pragma once

#include <cstddef>
#include <utility>

using std::swap;

template <typename T> const T& median_of_three(const T& a, const T& b, const T& c) {
        if ((a <= b && b <= c) || (a >= b && b >= c)) return b;
        if ((b <= a && a <= c) || (b >= a && a >= c)) return a;
        return c;
    }

template <typename T> int partition(T* array, int left, int right){
    T pivot = median_of_three(array[left], array[(left + right) / 2], array[right]);
    int i = left - 1;

    for(int j = left; j < right; j++){
        if (array[j] <= pivot){
            ++i;
            swap(array[i], array[j]);
        }
    }

    swap(array[i + 1], array[right]);
    return i + 1;
}

template <typename T> void quick_sort(T* array, int left, int right){
    if (right <= left) return;
    int pivot = partition(array, left, right);
    quick_sort<T>(array, left, pivot-1);
    quick_sort<T>(array, pivot+1, right);
}