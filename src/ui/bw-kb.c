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

#include "bw-kb.h"

/* For Raylib Keyboard Functions */
#include "../../lib/ui/raylib.h"

/* For memset() */
#include <string.h>

int get_key_press_list(char* keys, int max_num_keys) {
    memset(keys, 0, max_num_keys);
    for(int i = 0; i < max_num_keys; i++) {
        keys[i] = GetCharPressed();
        if(keys[i] == 0) return i + 1;
    }
    return 0; //max_num_keys was 0
}
