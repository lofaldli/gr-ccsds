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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include "ccsds_decoder_impl.h"
#include "ccsds.h"
#include "reed_solomon.h"

#define STATE_SYNC_SEARCH 0
#define STATE_CODEWORD 1

namespace gr {
  namespace ccsds {

    ccsds_decoder::sptr
    ccsds_decoder::make(int threshold, bool rs_decode, bool deinterleave, bool descramble, bool verbose, bool printing)
    {
      return gnuradio::get_initial_sptr
        (new ccsds_decoder_impl(threshold, rs_decode, deinterleave, descramble, verbose, printing));
    }

    ccsds_decoder_impl::ccsds_decoder_impl(int threshold, bool rs_decode, bool deinterleave, bool descramble, bool verbose, bool printing)
      : gr::sync_block("ccsds_decoder",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(0, 0, 0)),
        d_threshold(threshold),
        d_rs_decode(rs_decode),
        d_deinterleave(deinterleave),
        d_descramble(descramble),
        d_verbose(verbose),
        d_printing(printing),
        d_num_frames_received(0),
        d_num_frames_decoded(0),
        d_num_subframes_decoded(0)
    {
      message_port_register_out(pmt::mp("out"));

      for (uint8_t i=0; i<SYNC_WORD_LEN; i++) {
          d_sync_word = (d_sync_word << 8) | (SYNC_WORD[i] & 0xff);
      }

      enter_sync_search();
    }

    ccsds_decoder_impl::~ccsds_decoder_impl()
    {
    }

    int
    ccsds_decoder_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const uint8_t *in = (const uint8_t *) input_items[0];

      uint16_t count = 0;
      while (count < noutput_items) {
          switch (d_decoder_state) {
              case STATE_SYNC_SEARCH:
                  // get next bit
                  d_data_reg = (d_data_reg << 1) | (in[count++] & 0x01);
                  if (compare_sync_word()) {
                      if (d_verbose) printf("\tsync word detected\n");
                      d_num_frames_received++;
                      enter_codeword();
                  }
                  break;
              case STATE_CODEWORD:
                  // get next bit and pack then into full bytes
                  d_data_reg = (d_data_reg << 1) | (in[count++] & 0x01);
                  d_bit_counter++;
                  if (d_bit_counter == 8) {
                      d_codeword[d_byte_counter] = d_data_reg;
                      d_byte_counter++;
                      d_bit_counter = 0;
                  }
                  // once the full codeword is loaded, try to decode the packet
                  if (d_byte_counter == CODEWORD_LEN) {
                      if (d_verbose) printf("\tloaded codeword of length %i\n", CODEWORD_LEN);
                      if (d_printing) print_bytes(d_codeword, CODEWORD_LEN);

                      bool success = decode_frame();
                      if (success) {
                          pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL, pmt::make_blob(d_payload, DATA_LEN)));
                          message_port_pub(pmt::mp("out"), pdu);
                      }

                      if (d_verbose) {
                          printf("\tframes received: %i\n\tframes decoded: %i\n\tsubframes decoded: %i\n",
                                  d_num_frames_received,
                                  d_num_frames_decoded,
                                  d_num_subframes_decoded);
                      }
                      enter_sync_search();
                  }
                  break;
          }
      }
      return noutput_items;
    }

    void
    ccsds_decoder_impl::enter_sync_search()
    {
        if (d_verbose) printf("enter sync search\n");
        d_decoder_state = STATE_SYNC_SEARCH;
        d_data_reg = 0;
    }
    void
    ccsds_decoder_impl::enter_codeword()
    {
        if (d_verbose) printf("enter codeword\n");
        d_decoder_state = STATE_CODEWORD;
        d_byte_counter = 0;
        d_bit_counter = 0;
    }
    bool ccsds_decoder_impl::compare_sync_word()
    {
        uint32_t nwrong = 0;
        uint32_t wrong_bits = d_data_reg ^ d_sync_word;
        volk_32u_popcnt(&nwrong, wrong_bits);
        return nwrong <= d_threshold;
    }

    bool ccsds_decoder_impl::decode_frame()
    {
        // this will be set to false if a codeword is not decodable
        bool success = true;

        if (d_descramble) {
            descramble(d_codeword, CODEWORD_LEN);
        }

        // deinterleave and decode rs blocks
        uint8_t rs_block[RS_BLOCK_LEN];
        int8_t nerrors;
        for (uint8_t i=0; i<RS_NBLOCKS; i++) {
            for (uint8_t j=0; j<RS_BLOCK_LEN; j++) {

                if (d_deinterleave) {
                    rs_block[j] = d_codeword[i+(j*RS_NBLOCKS)];
                } else {
                    rs_block[j] = d_codeword[i*RS_BLOCK_LEN + j];
                }

            }
            if (d_rs_decode) {
                nerrors = d_rs.decode(rs_block);
                if (nerrors == -1) {
                    if (d_verbose) printf("\tcould not decode rs block #%i\n", i);
                    success = false;
                } else {
                    if (d_verbose) printf("\tdecoded rs block #%i with %i errors\n", i, nerrors);
                    d_num_subframes_decoded++;
                }
            }
            memcpy(&d_payload[i*RS_DATA_LEN], rs_block, RS_DATA_LEN);
        }

        if (success) d_num_frames_decoded++;

        return success;
    }
  } /* namespace ccsds */
} /* namespace gr */
