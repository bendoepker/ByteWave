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

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <types.h>

/*
*   Dynamic Array
*/
typedef struct {
    void* array;            //Pointer to the actual array
    int element_size;       //Size of one element in bytes
    int array_length;       //number of elements that are currently in the array
    int array_empty_space;  //Number of elements that can be added before expanding the array
    int array_capacity;     //Number of elements that can be stored (empty space + length)
} d_array;

BWError DArrayCreate(d_array* da, int element_size, int initial_capacity);
void DArrayDestroy(d_array* da);
//Add elements to the back of the array
void DArrayAppend(d_array* da, void* elements, int num_elements);
//Remove elements from the back of the array
void DArrayPopBack(d_array* da, int num_elements);
//Insert elements into the position at [index] and moves all elements at and after it back
void DArrayInsert(d_array* da, void* elements, int num_elements, int index);
//Remove elements at the position [index] and moves the elements after to the front
void DArrayRemove(d_array* da, int num_elements, int index);
//Search for an element in the array that matches 'element', only searches for a single element
//and returns -1 if not found, returns the index if found
int DArraySearch(d_array* da, void* element);
//Return the element at the index, if the index is beyond the last index it returns 0
void* DArrayAt(d_array* da, int index);

#endif //DYNAMIC_ARRAY_H
