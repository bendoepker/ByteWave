# Naming Convention 

## General (TL;DR)

> Anything that is exposed and used globally should be in pascal case using the prefix
> BW and their application, i.e.
> ```c
>   BWType var;
>   BWApi_Function();
>```

> Anything that is used internally in a function (variables, inline functions, etc)
> but is exposed globally should begin with an underscore and be snake case, i.e.
> ```c
>   _variable_name = x;
>   _function_name();
>```

## Functions

### Host API

> If a function is exposed globally it should be in Pascal Case using the format:
>
>```c
>    void BWHostApi_FunctionName() { /* Globally Exposed */ }
>```

> If a function is only used internally within the host api it should be snake case and use the format:
>
>```c
>    void _bw_hostapi_function_name() { /* Internally Used */ }
>```

### UI

> UI functions should follow the same format as Host API functions depending on their scope
> of exposure, but should have the prefix BWUI, i.e.
>```c
>   void BWUI_FunctionName() { /* Globally Exposed */ }
>   void _bw_ui_function_name() { /* Internally Used */ }
>```

### Macros

> All macros should have the BW_ prefix and should be all caps  
> Macros should not contain semicolons, semicolons should be put at the
> end in use
>```c
>   #define BW_MACRO(s) do_something(s)
>
>   int main() {
>       BW_MACRO("Hello World!");
>   }
>```

## Variables

### Local Variables

> If a variable has a scope limited to its retaining function it should be snake case
> and not have a bw or api prefix, it should be descriptive to its role and meaning
>```c
>   int example_function() {
>       int zero_result = 0;
>       return zero_result;
>   }
>```

## File Naming / Structure

### General

- Files should be named using dashes (-) between each word
- Markdown files should be ALLCAPS.md

### Host API

- All audio host API files should be contained in /src/hostapi/\<host-api-name\>/*
- All globally used functions, variables, types, etc. should be declared in bw-\<host-api-name\>.h
- These functions should then be defined in bw-\<host-api-name\>.c
- Any extra helper functions should be declared in bw-\<host-api-name\>-\<descriptor\>.h
- These functions should then be defined in bw-\<host-api-name\>-\<descriptor\>.c(pp)
