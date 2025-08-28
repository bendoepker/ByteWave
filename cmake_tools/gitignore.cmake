function(ignore_directory directory)
    file(WRITE ${directory}/.gitignore "*")
endfunction()
