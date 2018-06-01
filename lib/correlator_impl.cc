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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include "correlator_impl.h"

#if 1
#define debug_print printf
#else
#define debug_print
#endif

namespace gr {
  namespace ccsds {

    correlator::sptr
    correlator::make(const uint64_t asm_, const uint64_t asm_mask,
                     const uint8_t threshold, const size_t frame_len)
    {
      return gnuradio::get_initial_sptr
        (new correlator_impl(asm_, asm_mask, threshold, frame_len));
    }

    correlator_impl::correlator_impl(const uint64_t asm_, const uint64_t asm_mask,
                                     const uint8_t threshold, const size_t frame_len)
      : gr::sync_block("correlator",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(0, 0, 0)),
      d_asm(asm_), d_asm_mask(asm_mask),
      d_threshold(threshold), d_frame_len(frame_len),
      d_frame_count(0), d_ambiguity(NONE)
    {
        message_port_register_out(pmt::mp("out"));
        d_frame_buffer = (uint8_t *)malloc(frame_len * sizeof(uint8_t));
        enter_state(SEARCH);
    }

    correlator_impl::~correlator_impl() {
        free(d_frame_buffer);
    }

    int
    correlator_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        const uint8_t *in = (const uint8_t *) input_items[0];

        uint64_t count = 0;
        while (count < noutput_items) {
            switch (d_state) {
            case SEARCH:
                d_asm_buf = (d_asm_buf << 1) | (in[count++] & 0x01);
                if (check_asm(d_asm_buf)) {
                    d_ambiguity = NONE;
                    enter_state(LOCK);
                } else if (check_asm(d_asm_buf ^ 0xffffffffffffffff)) {
                    d_ambiguity = INVERTED;
                    enter_state(LOCK);
                }
                break;
            case LOCK:
                d_byte_buf = (d_byte_buf << 1) | (in[count++] & 0x01);
                d_bit_ctr++;
                if (d_bit_ctr == 8) {
                    if (d_ambiguity == NONE) {
                        d_frame_buffer[d_frame_buffer_len] = d_byte_buf;
                    } else if (d_ambiguity == INVERTED) {
                        d_frame_buffer[d_frame_buffer_len] = d_byte_buf ^ 0xff;
                    }
                    d_frame_buffer_len++;
                    d_bit_ctr = 0;
                }
                if (d_frame_buffer_len == d_frame_len) {
                    publish_msg();
                    d_frame_count++;
                    enter_state(SEARCH);
                }
                break;
            }
        }
        return noutput_items;
    }
    
    bool correlator_impl::check_asm(const uint64_t asm_buf) {
        debug_print("check_asm\n");

        uint64_t nerrors = 0;
        const uint64_t syndrome = (asm_buf ^ d_asm) & d_asm_mask;
        volk_64u_popcnt(&nerrors, syndrome);
        return nerrors <= (uint64_t)d_threshold;
    }

    void correlator_impl::enter_state(const state_t state) {
        debug_print("enter_state: %s\n", state_names[state]);

        switch (state) {
        case SEARCH:
            d_asm_buf = 0;
            break;
        case LOCK:
            d_byte_buf = 0;
            d_bit_ctr = 0;
            d_frame_buffer_len = 0;
            break;
        }
        d_state = state;
    }

    void correlator_impl::publish_msg() {
        debug_print("publish_msg #%lu\n", d_frame_count);

        pmt::pmt_t meta = pmt::make_dict();
        meta = dict_add(meta, pmt::intern("frame_count"), 
                        pmt::from_uint64(d_frame_count));
        const pmt::pmt_t data = pmt::init_u8vector(d_frame_len, d_frame_buffer);
        message_port_pub(pmt::mp("out"), pmt::cons(meta, data));
    }

  } /* namespace ccsds */
} /* namespace gr */

