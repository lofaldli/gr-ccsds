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

#include "reed_solomon.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

extern "C" {
#include "fec-3.0.1/fec.h"
}
#include "ccsds.h"

extern unsigned char CCSDS_alpha_to[];
extern unsigned char CCSDS_index_of[];
extern unsigned char CCSDS_poly[];
extern unsigned char Taltab[];
extern unsigned char Tal1tab[];

namespace gr {
    namespace ccsds {

        reed_solomon::reed_solomon() {}
        reed_solomon::~reed_solomon() {}

        void reed_solomon::encode(uint8_t *data) {
            encode_rs_ccsds(data, &data[RS_DATA_LEN],0);
        }
        int16_t reed_solomon::decode(uint8_t *data) {
            return decode_rs_ccsds(data, 0, 0, 0);
        }

    }
}
