find_package(libxmp 4.6.0 QUIET CONFIG)
if (NOT libxmp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        libxmp
        GIT_REPOSITORY https://github.com/libxmp/libxmp.git
        GIT_TAG libxmp-4.6.0
    )
    FetchContent_GetProperties(libxmp)
    if (NOT libxmp_POPULATED) # Have we downloaded libxmp yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(libxmp)

        add_subdirectory(${libxmp_SOURCE_DIR} ${libxmp_BINARY_DIR})
    endif()
endif()
