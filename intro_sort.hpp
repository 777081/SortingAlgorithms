#pragma once

#include "quick_sort.hpp"

#include <cstddef>
#include <utility>
#include <cmath>

using std::swap;

template <typename T> void insertion(T* array, int left, int right){
    for(int i = left + 1; i <= right; i++){
        T key = array[i];
        int j = i - 1;

        while(j >= left && array[j] > key){
            array[j+1] = array[j];
            --j;
        }

        array[j+1] = key;
    }

    return;
}

template <typename T> void heapify(T* array, int left, int right, int root){
    int largest = root;
    int l = 2*(root - left) + left + 1;
    int r = 2*(root - left) + left + 2;

    if(l<=right && array[l] > array[largest]){
        largest = l;

    }

    if(r<=right && array[r] > array[largest]){
        largest = r;
    }

    if(largest != root) {
        swap(array[root], array[largest]);
        heapify(array, left, right, largest);
    }
}

template <typename T> void heap_sort(T* array, int left, int right){
    if(right <= left) return;

    int n = right - left + 1;

    for (int i = left + n/2; i-- > left;){
        heapify(array, left, right, i);
        if (i == left) break;
    }

    for(int i = right; i > left; i--){
        swap(array[left], array[i]);
        heapify(array, left, i-1, left);
    }
}

template <typename T> void intro_sort_util(T* array, int left, int right, int max_depth){
    if(right <= left) return;

    int n = right - left + 1;

    if(n < 16){
        insertion(array, left, right);
        return;
    }

    if(max_depth == 0){
        heap_sort(array, left, right);
        return;
    }

    int partition = partition(array, left, right);

    intro_sort_util(array, left, partition, max_depth - 1);
    intro_sort_util(array, partition + 1, right, max_depth - 1);
}