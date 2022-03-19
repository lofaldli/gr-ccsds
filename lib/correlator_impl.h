/* -*- c++ -*- */
/* 
 * Copyright 2018 André Løfaldli.
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

#ifndef INCLUDED_CCSDS_CORRELATOR_IMPL_H
#define INCLUDED_CCSDS_CORRELATOR_IMPL_H

#include <gnuradio/ccsds/correlator.h>
#include <vector>

namespace gr {
  namespace ccsds {

    class correlator_impl : public correlator {
      private:
        const uint64_t d_asm, d_asm_mask;
        const uint8_t d_threshold;
        const size_t d_frame_len;

        uint64_t d_asm_buf;
        uint8_t *d_frame_buffer;
        size_t d_frame_buffer_len;
        uint8_t d_bit_ctr, d_byte_buf;
        
        state_t d_state;
        ambiguity_t d_ambiguity;
        uint64_t d_frame_count;

      public:
        correlator_impl(const uint64_t asm_, const uint64_t asm_mask, 
                        const uint8_t threshold, const size_t frame_len);
        ~correlator_impl(); 
        int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

        bool check_asm(const uint64_t asm_buf);
        void enter_state(const state_t state);
        void publish_msg();

        uint64_t frame_count() const;
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CORRELATOR_IMPL_H */

