#pragma once

#include <cstddef>
#include <utility>

template <typename T> int partition(T* array, int left, int right){
    T pivot = array[right];  
    int i = left - 1;

    for(int j = left, j < right; j++){
        if (array[j] <= pivot){
            ++i;
            swap(array[i], array[j]);
        }
    }

    swap(array[i + 1], array[right]);
    return i + 1;
}

template <typename T> void quick_sort(int* array, int left, int right){
    if (right < left) return;
    int pivot = partition(array, left, right);
    quick_sort(array, left, pivot-1);
    quick_sort(array, pivot+1, right);
}