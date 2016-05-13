INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CCSDS ccsds)

FIND_PATH(
    CCSDS_INCLUDE_DIRS
    NAMES ccsds/api.h
    HINTS $ENV{CCSDS_DIR}/include
        ${PC_CCSDS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    CCSDS_LIBRARIES
    NAMES gnuradio-ccsds
    HINTS $ENV{CCSDS_DIR}/lib
        ${PC_CCSDS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CCSDS DEFAULT_MSG CCSDS_LIBRARIES CCSDS_INCLUDE_DIRS)
MARK_AS_ADVANCED(CCSDS_LIBRARIES CCSDS_INCLUDE_DIRS)

