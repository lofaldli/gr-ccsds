/* -*- c++ -*- */
/*
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "ccsds_encoder_impl.h"

#include "ccsds.h"
#include "reed_solomon.h"

namespace gr {
  namespace ccsds {

    ccsds_encoder::sptr
    ccsds_encoder::make(size_t itemsize, const std::string& len_tag_key, bool rs_encode, bool interleave, bool scramble, bool printing, bool verbose)
    {
      return gnuradio::get_initial_sptr
        (new ccsds_encoder_impl(itemsize, len_tag_key, rs_encode, interleave, scramble, printing, verbose));
    }

    /*
     * The private constructor
     */
    ccsds_encoder_impl::ccsds_encoder_impl(size_t itemsize, const std::string& len_tag_key, bool rs_encode, bool interleave, bool scramble, bool printing, bool verbose)
      : gr::tagged_stream_block("ccsds_encoder",
              gr::io_signature::make(itemsize==0 ? 0:1, itemsize==0 ? 0:1, itemsize),
              gr::io_signature::make(1, 1, sizeof(uint8_t)), "packet_len"),
        d_itemsize(itemsize),
        d_rs_encode(rs_encode),
        d_interleave(interleave),
        d_scramble(scramble),
        d_printing(printing),
        d_verbose(verbose),
        d_curr_len(0),
        d_num_packets(0)
    {
      if (d_itemsize == 0) {
          message_port_register_in(pmt::mp("in"));
      }

      memcpy(d_pkt.sync_word, SYNC_WORD, SYNC_WORD_LEN);
    }

    /*
     * Our virtual destructor.
     */
    ccsds_encoder_impl::~ccsds_encoder_impl()
    {

    }

    int
    ccsds_encoder_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
        // copy from message queue
        if (d_itemsize == 0) {

            if (d_curr_len != 0) return 0;

            pmt::pmt_t msg(delete_head_blocking(pmt::mp("in"), 100));
            if (msg.get() == NULL) {
                return 0;
            }
            if (!pmt::is_pair(msg)) {
                throw std::runtime_error("received a malformed pdu message");
            }
            d_curr_meta = pmt::car(msg);
            d_curr_vec = pmt::cdr(msg);
            d_curr_len = pmt::length(d_curr_vec);
        }
        return TOTAL_FRAME_LEN;
    }

    int
    ccsds_encoder_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {

      const uint8_t* in;
      if (d_itemsize == 0) {
          // see if there is anything to do
          if (d_curr_len == 0) return 0;

          if (d_curr_len != DATA_LEN) {
              printf("[ERROR] expected %i bytes, got %i\n", DATA_LEN, (int)d_curr_len);
              d_curr_len = 0;
              return 0;
          }

          size_t len = 0;
          in = (const uint8_t*) uniform_vector_elements(d_curr_vec, len);
      } else {
          in = (const uint8_t*) input_items[0];
      }
      uint8_t *out = (uint8_t *) output_items[0];
      //copy_stream_tags();

      uint8_t rs_block[RS_BLOCK_LEN];
      for (uint8_t i=0; i<RS_NBLOCKS; i++) {

          // copy data from input to rs block
          memcpy(rs_block, &in[i*RS_DATA_LEN], RS_DATA_LEN);

          // calculate parity data
          if (d_rs_encode) {
              d_rs.encode(rs_block);
          } else {
              memset(&rs_block[RS_DATA_LEN], 0, RS_PARITY_LEN);
          }

          // data into output array
          if (d_interleave) {
              for (uint8_t j=0; j<RS_BLOCK_LEN; j++)
                  d_pkt.codeword[i + (RS_NBLOCKS*j)] = rs_block[j];
          } else {
              memcpy(&d_pkt.codeword[i*RS_BLOCK_LEN], rs_block, RS_BLOCK_LEN);
          }
      }

      if (d_scramble) {
          scramble(d_pkt.codeword, CODEWORD_LEN);
      }

      d_num_packets++;
      if (d_verbose) {
          printf("[ENCODER] sending %i bytes of data\n", TOTAL_FRAME_LEN);
          printf("[ENCODER] number of packets transmitted: %i\n", d_num_packets);
      }

      if (d_printing) {
          print_bytes(d_pkt.codeword, CODEWORD_LEN);
      }

      // copy data into output array
      memcpy(out, &d_pkt, TOTAL_FRAME_LEN);

      // reset state
      d_curr_len = 0;
      return TOTAL_FRAME_LEN;
    }

    void
    ccsds_encoder_impl::copy_stream_tags()
    {
        if (!pmt::eq(d_curr_meta, pmt::PMT_NIL)) {
            pmt::pmt_t klist(pmt::dict_keys(d_curr_meta));
            for (size_t i=0; i<pmt::length(klist); i++) {
                pmt::pmt_t k(pmt::nth(i, klist));
                pmt::pmt_t v(pmt::dict_ref(d_curr_meta, k, pmt::PMT_NIL));
                add_item_tag(0, nitems_written(0), k, v, alias_pmt());
            }
        }
    }

  } /* namespace ccsds */
} /* namespace gr */

