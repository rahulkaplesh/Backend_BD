set(TUFAO_MAJ_VERSION "1")
if (TUFAO1_ROOT)
    message("Found TUFAO1_ROOT")
    if (MSVC)
        set(CONFIGURATION $(Configuration))
    else()
        set(CONFIGURATION ${CMAKE_BUILD_TYPE})
    endif()

    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX "d")
    elseif(APPLE)
        set(CMAKE_DEBUG_POSTFIX "_debug")
    endif()

    GET_FILENAME_COMPONENT(TUFAO_INCLUDE_DIR "${TUFAO1_ROOT}/include/tufao-${TUFAO_MAJ_VERSION}" ABSOLUTE)

    set(TUFOA_LIB "tufao1")

    if(WIN32)
        set(prefix "")
        set(suffix ".lib")
    else()
        set(prefix "lib")
        set(suffix ".so")
    endif()

    set(TUFAO_BINARIES ${TUFAO1_ROOT}/bin/tufao1.dll)
    set(TUFAO_DEBUG_BINARIES ${TUFAO1_ROOT}/bin/tufao1${CMAKE_DEBUG_POSTFIX}.dll)

    file(COPY ${TUFAO_BINARIES}
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${TUFAO_DEBUG_BINARIES}
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    set(TUFAO_DEBUG_LIB ${TUFAO1_ROOT}/lib/${prefix}tufao1${CMAKE_DEBUG_POSTFIX}${suffix})
    set(TUFAO_LIB ${TUFAO1_ROOT}/lib/${prefix}tufao1${suffix})

    message(${TUFAO_LIB})
else()
    message(FATAL_ERROR "Could not find TUFAO1_ROOT")
endif()