# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT ccsds_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT ccsds_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND ccsds_python_files
    ccsds_decoder_python.cc
    ccsds_encoder_python.cc
    correlator_python.cc
    python_bindings.cc)

GR_PYBIND_MAKE_OOT(ccsds
   ../../..
   gr::ccsds
   "${ccsds_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/ccsds/
  DEPENDS ccsds_python)

install(TARGETS ccsds_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/ccsds COMPONENT pythonapi)
