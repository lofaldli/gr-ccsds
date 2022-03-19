#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 André Løfaldli.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

import time
import random
from gnuradio import gr, gr_unittest
from gnuradio import blocks, digital
import pmt
import ccsds_python as ccsds

class qa_correlator (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        asm = (0x1a, 0xcf, 0xfc, 0x1d)
        frame_len = 223
        random_data = tuple(random.randint(0, 255) for _ in range(frame_len))

        data_in = asm + random_data
        
        src = blocks.vector_source_b(data_in, repeat=True)
        unpack = blocks.unpack_k_bits_bb(8)
        corr = ccsds.correlator(0x1acffc1d, 0xffffffff, 0, frame_len)
        dbg = blocks.message_debug()
        self.tb.connect(src, unpack, corr)
        self.tb.msg_connect((corr, 'out'), (dbg, 'store'))
        self.tb.start()

        while dbg.num_messages() < 1:
            time.sleep(0.001)

        self.tb.stop()
        self.tb.wait()

        msg = dbg.get_message(0)
        data_out = tuple(pmt.to_python(pmt.cdr(msg)))
        
        assert frame_len == len(data_out)
        assert random_data == data_out

    def test_002_inverted_bits (self):
        asm = (0x1a, 0xcf, 0xfc, 0x1d)
        frame_len = 223
        random_data = tuple(random.randint(0, 255) for _ in range(frame_len))

        data_in = asm + random_data
        
        src = blocks.vector_source_b(data_in, repeat=True)
        unpack = blocks.unpack_k_bits_bb(8)
        mapper = digital.map_bb((1,0))
        corr = ccsds.correlator(0x1acffc1d, 0xffffffff, 0, frame_len)
        dbg = blocks.message_debug()
        self.tb.connect(src, unpack, mapper, corr)
        self.tb.msg_connect((corr, 'out'), (dbg, 'store'))
        self.tb.start()

        while dbg.num_messages() < 1:
            time.sleep(0.001)

        self.tb.stop()
        self.tb.wait()

        msg = dbg.get_message(0)
        data_out = tuple(pmt.to_python(pmt.cdr(msg)))
        
        assert frame_len == len(data_out)
        assert random_data == data_out

    def test_003_asm_mask (self):
        # asm with errors
        asm = (0x1f, 0xcf, 0xf0, 0x1d)
        frame_len = 223
        random_data = tuple(random.randint(0, 255) for _ in range(frame_len))

        data_in = asm + random_data
        
        src = blocks.vector_source_b(data_in, repeat=True)
        unpack = blocks.unpack_k_bits_bb(8)
        mapper = digital.map_bb((1,0))
        # mask to ignore errors
        corr = ccsds.correlator(0x1acffc1d, 0xf0fff0ff, 0, frame_len)
        dbg = blocks.message_debug()
        self.tb.connect(src, unpack, mapper, corr)
        self.tb.msg_connect((corr, 'out'), (dbg, 'store'))
        self.tb.start()

        while dbg.num_messages() < 1:
            time.sleep(0.001)

        self.tb.stop()
        self.tb.wait()

        msg = dbg.get_message(0)
        data_out = tuple(pmt.to_python(pmt.cdr(msg)))
        
        assert frame_len == len(data_out)
        assert random_data == data_out

    def test_004_threshold (self):
        # asm with a few bits wrong
        asm = (0x3a, 0xcf, 0xec, 0x1d)
        frame_len = 223
        random_data = tuple(random.randint(0, 255) for _ in range(frame_len))

        data_in = asm + random_data
        
        src = blocks.vector_source_b(data_in, repeat=True)
        unpack = blocks.unpack_k_bits_bb(8)
        mapper = digital.map_bb((1,0))
        # threshold set to 2
        corr = ccsds.correlator(0x1acffc1d, 0xffffffff, 2, frame_len)
        dbg = blocks.message_debug()
        self.tb.connect(src, unpack, mapper, corr)
        self.tb.msg_connect((corr, 'out'), (dbg, 'store'))
        self.tb.start()

        while dbg.num_messages() < 1:
            time.sleep(0.001)

        self.tb.stop()
        self.tb.wait()

        msg = dbg.get_message(0)
        data_out = tuple(pmt.to_python(pmt.cdr(msg)))
        
        assert frame_len == len(data_out)
        assert random_data == data_out

if __name__ == '__main__':
    gr_unittest.run(qa_correlator, "qa_correlator.xml")
