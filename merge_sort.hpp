#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector, std::copy, std::min;

template <typename T> void merge(T* array, int left, int mid, int right){
    int l_size = mid - left + 1;
    int r_size = right - mid;

    vector<T> l_arr(l_size);
    vector<T> r_arr(r_size);

    copy(array + left, array + mid + 1, l_arr.begin());
    copy(array + mid + 1, array + right + 1, r_arr.begin());

    int i = 0, j = 0, k = left;

    while (i < l_size && j < r_size){
        if(l_arr.at(i) <= r_arr[j]){
            array[k] = l_arr[i];
            ++i;
        }
        else{
            array[k] = r_arr[j];
            ++j;
        }
        ++k;
    }

    while (i < l_size){
        array[k] = l_arr[i];
        ++i;
        ++k;
    }

    while (j < r_size){
        array[k] = r_arr[j];
        ++j;
        ++k;
    }
}

template <typename T> void merge_sort(T* array, int left, int right){
    if (left >= right) return;

    int mid = left + (right - left)/2;

    merge_sort(array, left, mid);
    merge_sort(array, mid+1, right);
    merge(array, left, mid, right);
}
