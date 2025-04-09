# TODO

- finish the interface layer for asio.h functions, ensure error casting
    from ASIOError to BWError is present
- move non-error return values into function params so that the return value
    can be solely responsible for the error state
