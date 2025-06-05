#ifndef MIXER_H
#define MIXER_H
/*
*
*   The Mixer Track
*
*   Mixer tracks have different input types which should be handled in different ways
*
*   External Input: mixer tracks that have an input coming from an external source
*   (host api input buffer) should immediately call the begin_mixer() function to begin
*   processing the buffer
*
*   Mixer Input: mixer tracks which have an input coming from another mixer track should
*   wait until the processing is finished for each previous mixer track before processing
*
*   Convergent Mixer: requires all previous mixers to be finished outputting before 
*
*
*
*/
#define MIXER_MAX_EFFECTS 10

typedef struct Mixer Mixer;
struct Mixer {
    int done_processing;
    int buffer_size;
    float* left_buffer;
    float* right_buffer;
    int num_mixer_inputs;
    int num_mixer_outputs;
    Mixer* mixer_inputs;
    Mixer* mixer_outputs;
    void (*begin_mixer)(Mixer);
    float gain_level; //Value ranges from 0.0 to 1.25
    float pan; //Value ranges from -1.0 (All left) to 1.0 (All right)
};

//Process audio
void BWMixer_Begin(Mixer* mixer);

//Create and destroy {mixer} object (creates with defaulted parameters)
void BWMixer_Create(Mixer** mixer);
void BWMixer_Destroy(Mixer* mixer);

//Add / remove {mixer_input} route to {mixer}
void BWMixer_AddInput(Mixer* mixer, Mixer* mixer_input);
void BWMixer_RemoveInput(Mixer* mixer, Mixer* mixer_input);

//Add / remove {mixer_output} routes from {mixer}
void BWMixer_AddOutput(Mixer* mixer, Mixer* mixer_output);
void BWMixer_RemoveOutput(Mixer* mixer, Mixer* mixer_output);

//Adjust the gain level of the mixer
void BWMixer_AdjustGain(Mixer* mixer, float gain_level);

//Adjust the pan level of the mixer
void BWMixer_AdjustPan(Mixer* mixer, float pan);

#endif //MIXER_H
