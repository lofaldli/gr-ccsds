/* -*- c++ -*- */
/*
 * Copyright 2016 André Løfaldli.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_CCSDS_CCSDS_DECODER_IMPL_H
#define INCLUDED_CCSDS_CCSDS_DECODER_IMPL_H

#include <ccsds/ccsds_decoder.h>
#include "ccsds.h"
#include "reed_solomon.h"

namespace gr {
  namespace ccsds {

    class ccsds_decoder_impl : public ccsds_decoder
    {
     private:
         uint8_t d_threshold;
         bool d_rs_decode;
         bool d_deinterleave;
         bool d_descramble;
         bool d_verbose;
         bool d_printing;

         uint32_t d_sync_word;
         uint8_t d_decoder_state;
         uint32_t d_data_reg;
         uint8_t d_bit_counter;
         uint16_t d_byte_counter;
         uint32_t d_num_frames_received;
         uint32_t d_num_frames_decoded;
         uint32_t d_num_subframes_decoded;
         uint8_t d_codeword[CODEWORD_LEN];
         uint8_t d_payload[DATA_LEN];
         reed_solomon d_rs;

         void enter_sync_search();
         void enter_codeword();
         bool compare_sync_word();
         bool decode_frame();

     public:
      ccsds_decoder_impl(int threshold, bool rs_decode, bool deinterleave, bool descramble, bool verbose, bool printing);
      ~ccsds_decoder_impl();

      uint32_t num_frames_received() const {return d_num_frames_received;}
      uint32_t num_frames_decoded() const {return d_num_frames_decoded;}
      uint32_t num_subframes_decoded() const {return d_num_subframes_decoded;}

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_DECODER_IMPL_H */

