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

#ifndef INCLUDED_CCSDS_CCSDS_ENCODER_IMPL_H
#define INCLUDED_CCSDS_CCSDS_ENCODER_IMPL_H

#include <ccsds/ccsds_encoder.h>
#include "ccsds.h"
#include "reed_solomon.h"

namespace gr {
  namespace ccsds {

    class ccsds_encoder_impl : public ccsds_encoder
    {
     private:
         size_t d_itemsize;
         bool d_rs_encode;
         bool d_interleave;
         bool d_scramble;
         bool d_printing;
         bool d_verbose;

         uint32_t d_num_frames;

         pmt::pmt_t d_curr_meta;
         pmt::pmt_t d_curr_vec;
         size_t d_curr_len;

         struct ccsds_tx_pkt d_pkt;
         reed_solomon d_rs;

         void copy_stream_tags();

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      ccsds_encoder_impl(size_t itemsize, const std::string& len_tag_key, bool rs_encode, bool interleave, bool scramble, bool printing, bool verbose);
      ~ccsds_encoder_impl();

      uint32_t num_frames() const {return d_num_frames;}

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_ENCODER_IMPL_H */

