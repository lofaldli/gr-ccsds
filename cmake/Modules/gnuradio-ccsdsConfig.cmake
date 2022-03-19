find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_CCSDS gnuradio-ccsds)

FIND_PATH(
    GR_CCSDS_INCLUDE_DIRS
    NAMES gnuradio/ccsds/api.h
    HINTS $ENV{CCSDS_DIR}/include
        ${PC_CCSDS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_CCSDS_LIBRARIES
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

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-ccsdsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_CCSDS DEFAULT_MSG GR_CCSDS_LIBRARIES GR_CCSDS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_CCSDS_LIBRARIES GR_CCSDS_INCLUDE_DIRS)
