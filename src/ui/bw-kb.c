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
