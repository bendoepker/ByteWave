/*
*	Bytewave: A Lightweight Digital Audio Workstation
*	Copyright (C) 2025  Ben Doepker
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <bw-dynamic-array.h>
#include <stdlib.h>
#include <string.h>

/*
*   Dynamic Array
*/

BWError DArrayCreate(d_array* da, int element_size, int initial_capacity) {
    da->array = malloc(element_size * initial_capacity);
    if(da->array == 0) return BW_FAILED_MALLOC;

    memset(da->array, 0, initial_capacity * element_size);
    da->element_size = element_size;
    da->array_empty_space = initial_capacity;
    da->array_capacity = initial_capacity;
    da->array_length = 0;

    return BW_OK;
}

void DArrayDestroy(d_array* da) {
    free(da->array);
    da->array = 0;
}

void DArrayAppend(d_array* da, void* elements, int num_elements) {
    //Two cases: array has enough space -> store the elements in the array
    //           array doesn't have enough space -> double the size of the array
    if(da->array_empty_space >= num_elements) {
        //Case 1
        void* insertion_point = da->array + (da->array_length * da->element_size);
        memcpy(insertion_point, elements, num_elements * da->element_size);

        da->array_length += num_elements;
        da->array_empty_space -= num_elements;
    } else {
        //Case 2
        //Array expansion and copy
        while(da->array_capacity < (da->array_length + num_elements)) da->array_capacity *= 2;
        void* temp_array = malloc(da->array_capacity * da->element_size);
        memset(temp_array, 0, (da->array_capacity * da->element_size));
        memcpy(temp_array, da->array, da->array_length);
        free(da->array);
        da->array = temp_array;

        //Insertion
        void* insertion_point = da->array + (da->array_length * da->element_size);
        memcpy(insertion_point, elements, num_elements * da->element_size);

        //Update
        da->array_length += num_elements;
        da->array_empty_space = da->array_capacity - da->array_length;
    }
}

void DArrayPopBack(d_array* da, int num_elements) {
    if(num_elements >= da->array_length) {
        memset(da->array, 0, da->array_length);
        da->array_length = 0;
        da->array_empty_space = da->array_capacity;
    } else {
        memset((da->array + (da->array_length - num_elements) * da->element_size),
               0, num_elements * da->element_size);
        da->array_length -= num_elements;
        da->array_empty_space += num_elements;
    }
}

void DArrayInsert(d_array* da, void* elements, int num_elements, int index) {
    if(da->array_empty_space >= num_elements * da->element_size) {
        memmove(da->array + (index + num_elements) * da->element_size,
                da->array + index * da->element_size,
                (da->array_length - index) * da->element_size);
        memcpy(da->array + index * da->element_size, elements, num_elements);
        da->array_length += num_elements;
        da->array_empty_space -= num_elements;
    } else {
        while(da->array_capacity < (da->array_length + num_elements)) da->array_capacity *= 2;
        void* temp_array = malloc(da->array_capacity * da->element_size);
        memset(temp_array, 0, (da->array_capacity * da->element_size));
        //Copy the bytes before the index
        memcpy(temp_array, da->array, index * da->element_size);
        //Copy the bytes from elements
        memcpy(temp_array + index * da->element_size, elements, num_elements * da->element_size);
        //Copy the bytes after the index
        memcpy(temp_array + (index + num_elements) * da->element_size,
               da->array + index * da->element_size, (da->array_length - index) * da->element_size);
        free(da->array);
        da->array = temp_array;

        da->array_length += num_elements;
        da->array_empty_space = da->array_capacity - da->array_length;
    }
}

void DArrayRemove(d_array* da, int num_elements, int index) {
    memmove(da->array + index * da->element_size,
            da->array + (index + num_elements) * da->element_size,
            da->array_length - index - num_elements);
    da->array_length -= num_elements;
    da->array_empty_space += num_elements;
    memset(da->array + da->array_length * da->element_size, 0, num_elements);
}

int DArraySearch(d_array* da, void* element) {
    for(int i = 0; i < da->array_length; i++) {
        if(memcmp(da->array + i * da->element_size, element, da->element_size) == 0) return i;
    }
    return -1;
}

void* DArrayAt(d_array* da, int index) {
    if(index >= da->array_length) return 0;
    return da->array + (da->element_size * index);
}
