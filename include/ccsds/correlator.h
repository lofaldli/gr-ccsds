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


#ifndef INCLUDED_CCSDS_CORRELATOR_H
#define INCLUDED_CCSDS_CORRELATOR_H

#include <ccsds/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief CCSDS correlator
     * \ingroup ccsds
     *
     * looks for the attached sync marker and produces pdus containing frames
     */
    class CCSDS_API correlator : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<correlator> sptr;

      /*!
       * \brief make the correlator.
       *
       * \param asm attached sync marker
       * \param asm_mask mask for attached sync marker
       * \param threshold maximum number of allowed errors in asm
       * \param frame_len length of the transfer frame
       */
      static sptr make(const uint64_t asm_=0x1acffc1d,
                       const uint64_t asm_mask=0xffffffff, 
                       const uint8_t threshold=2, 
                       const size_t frame_len=223);

      /*!
       * \brief number of frames detected
       */
      virtual uint64_t frame_count() const = 0;

    };

    enum state_t { SEARCH, LOCK };
    const char* state_names[] = { "SEARCH", "LOCK" };

    enum ambiguity_t { NONE, INVERTED };
    const char* ambiguity_names[] = { "NONE", "INVERTED" };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CORRELATOR_H */

