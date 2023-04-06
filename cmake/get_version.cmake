cmake_minimum_required(VERSION 3.19)

function(pull_version)

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY "${CMAKE_PROJECT_DIR}"
    OUTPUT_VARIABLE GIT_TAG
    RESULTS_VARIABLE GIT_TAG_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REGEX MATCH "v.*\..*\..*" SEMVER_FOUND ${GIT_TAG})

if("${SEMVER_FOUND}" STREQUAL "")
    set(GIT_VER_SEM "v0.0.0-dev")
    set(GIT_VER_STR "0.0.0-dev")
    set(GIT_VER_MAJOR 0)
    set(GIT_VER_MINOR 0)
    set(GIT_VER_BUILD 0)
    set(GIT_VER_TAIL "dev")
endif()

string(REGEX MATCHALL "[0-9+]\\.|[0-9+]\\-" VER_NUMBERS ${GIT_TAG})

list(GET VER_NUMBERS 0 GIT_VER_MAJOR)
list(GET VER_NUMBERS 1 GIT_VER_MINOR)
list(GET VER_NUMBERS 2 GIT_VER_BUILD)
list(GET VER_NUMBERS 3 GIT_VER_TAIL)
string(REGEX REPLACE "([0-9+])\\." "\\1" GIT_VER_MAJOR ${GIT_VER_MAJOR})
string(REGEX REPLACE "([0-9+])\\." "\\1" GIT_VER_MINOR ${GIT_VER_MINOR})
string(REGEX REPLACE "([0-9+])\\-" "\\1" GIT_VER_BUILD ${GIT_VER_BUILD})
string(REGEX REPLACE "([0-9+])\\-" "\\1" GIT_VER_TAIL ${GIT_VER_TAIL})

string(JOIN "." GIT_VER_BASE ${GIT_VER_MAJOR} ${GIT_VER_MINOR} ${GIT_VER_BUILD})
string(JOIN "+" GIT_VER_STR ${GIT_VER_BASE} ${GIT_VER_TAIL})
string(JOIN "" GIT_VER_SEM "v" ${GIT_VER_STR})
message("Git describe: ${GIT_TAG}")
message("GIT_VER_SEM: ${GIT_VER_SEM}")
message("GIT_VER_STR: ${GIT_VER_STR}")
message("GIT_VER_MAJOR: ${GIT_VER_MAJOR}")
message("GIT_VER_MINOR: ${GIT_VER_MINOR}")
message("GIT_VER_BUILD: ${GIT_VER_BUILD}")
message("GIT_VER_TAIL: ${GIT_VER_TAIL}")

endfunction()