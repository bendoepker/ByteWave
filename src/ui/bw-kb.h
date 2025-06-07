#ifndef BW_KB_H
#define BW_KB_H

/*
*   returns the number of keys pressed
*   fills the 'keys' array up to max_num_keys or until all keys are accounted for with the currently pressed keys
*
*/
int get_key_press_list(char* keys, int max_num_keys);

#endif //BW_KB_H
