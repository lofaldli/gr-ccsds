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


#ifndef INCLUDED_CCSDS_CCSDS_ENCODER_H
#define INCLUDED_CCSDS_CCSDS_ENCODER_H

#include <gnuradio/ccsds/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace ccsds {

    /*!
     * \brief Create Reed Solomon Encoded CCSDS frames
     * \ingroup ccsds
     *
     * Description of the block
     *
     */
    class CCSDS_API ccsds_encoder : virtual public gr::tagged_stream_block
    {
     public:
      typedef std::shared_ptr<ccsds_encoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ccsds::ccsds_encoder.
       *
       */
      static sptr make(size_t itemsize=0, const std::string& len_tag_key="packet_len", bool rs_encode=true, bool interleave=true, bool scramble=true, bool printing=false, bool verbose=false, int n_interleave=5, bool dual_basis=true);

      /*!
       * \brief return the number of frames sent
       *
       */
      virtual uint32_t num_frames() const = 0;
    };

  } // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_ENCODER_H */

