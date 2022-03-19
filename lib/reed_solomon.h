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


#ifndef INCLUDED_REED_SOLOMON_H
#define INCLUDED_REED_SOLOMON_H

#include <gnuradio/ccsds/api.h>
#include <stdint.h>
#include <stdbool.h>

namespace gr {
    namespace ccsds {

        class CCSDS_API reed_solomon {
            private:
            public:
                reed_solomon();
                ~reed_solomon();

                void encode(uint8_t *data, bool use_dual_basis);
                int16_t decode(uint8_t *data, bool use_dual_basis);
        };

    }
}

#endif /* INCLUDED_REED_SOLOMON_H */
