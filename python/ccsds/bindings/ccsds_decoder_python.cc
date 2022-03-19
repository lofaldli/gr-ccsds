/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(ccsds_decoder.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(05c854ff374f104d8802cc3afddb3527)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/ccsds/ccsds_decoder.h>
// pydoc.h is automatically generated in the build directory
#include <ccsds_decoder_pydoc.h>

void bind_ccsds_decoder(py::module& m)
{

    using ccsds_decoder    = ::gr::ccsds::ccsds_decoder;


    py::class_<ccsds_decoder, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<ccsds_decoder>>(m, "ccsds_decoder", D(ccsds_decoder))

        .def(py::init(&ccsds_decoder::make),
           py::arg("threshold") = 0,
           py::arg("rs_decode") = true,
           py::arg("descramble") = true,
           py::arg("deinterleave") = true,
           py::arg("verbose") = false,
           py::arg("printing") = false,
           py::arg("n_interleave") = 5,
           py::arg("dual_basis") = true,
           D(ccsds_decoder,make)
        )
        




        
        .def("num_frames_received",&ccsds_decoder::num_frames_received,       
            D(ccsds_decoder,num_frames_received)
        )


        
        .def("num_frames_decoded",&ccsds_decoder::num_frames_decoded,       
            D(ccsds_decoder,num_frames_decoded)
        )


        
        .def("num_subframes_decoded",&ccsds_decoder::num_subframes_decoded,       
            D(ccsds_decoder,num_subframes_decoded)
        )

        ;




}








