/* -*- c++ -*- */

#define CCSDS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ccsds_swig_doc.i"

%{
#include "ccsds/ccsds_encoder.h"
#include "ccsds/ccsds_decoder.h"
#include "ccsds/correlator.h"
%}


%include "ccsds/ccsds_encoder.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, ccsds_encoder);
%include "ccsds/ccsds_decoder.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, ccsds_decoder);
%include "ccsds/correlator.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, correlator);
