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


#ifndef INCLUDED_CCSDS_CCSDS_DECODER_H
#define INCLUDED_CCSDS_CCSDS_DECODER_H

#include <ccsds/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief Decodes Reed Solomon encoded CCSDS frames
     * \ingroup ccsds
     *
     */
    class CCSDS_API ccsds_decoder : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<ccsds_decoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ccsds::ccsds_decoder.
       *
       */
      static sptr make(int threshold=0, bool rs_decode=true, bool descramble=true, bool deinterleave=true, bool verbose=false, bool printing=false);

      /*!
       * \brief return number of received frames
       */
      virtual uint32_t num_frames_received() const = 0;
      /*!
       * \brief return number of decoded frames
       */
      virtual uint32_t num_frames_decoded() const = 0;
      /*!
       * \brief return number of decoded subframes
       */
      virtual uint32_t num_subframes_decoded() const = 0;

    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_DECODER_H */

