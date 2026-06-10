function(set_project_warnings target)
    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Werror
    )
    set(GCC_WARNINGS ${CLANG_WARNINGS})
    target_compile_options(${target} INTERFACE ${CLANG_WARNINGS})
endfunction()
