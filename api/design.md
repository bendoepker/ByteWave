Audio API Design
================

## Core Responsibilities

- ### Create a unified way for the main audio process block to fill the audio buffers across different systems
- ### Route system errors to the error handlers


## Process-Trace Diagram

```
AudioProccesFunction
|
+---- Audio ----> BWAAPI +-> WASAPI
|                        |
|                        +-> ASIO
|                        |
|                        +-> CoreAudio
|                        |
|                        +-> PulseAudio
|                        |
|                        +-> PipeWire
|                        |
|                        +-> JACK
|
+---- Request ----> BWAAPI ----> Data ----> BWHandlers

WASAPI -----+----> Error ----> BWAAPI +-> BWErrorHandler
            |
ASIO -------+
            |
CoreAudio --+
            |
PulseAudio -+
            |
PipeWire ---+
            |
JACK -------+
```

