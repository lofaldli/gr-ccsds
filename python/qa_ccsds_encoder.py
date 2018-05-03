#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2016 André Løfaldli.
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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import ccsds_swig as ccsds
import pmt, time

class qa_ccsds_encoder (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

#    def test_001_t (self):
#        port = pmt.intern("in")
#        enc = ccsds.ccsds_encoder()
#        dec = ccsds.ccsds_decoder()
#        dbg = blocks.message_debug()
#        self.tb.connect(enc, dec)
#        self.tb.msg_connect(dec, "out", dbg, "store")
#
#        src_data = [x%256 for x in range(1115)]
#        src_vec = pmt.init_u8vector(len(src_data), src_data)
#        msg = pmt.cons(pmt.PMT_NIL, src_vec)
#
#        self.tb.start()
#        enc.to_basic_block()._post(port, msg)
#
#        while dbg.num_messages() < 1:
#            time.sleep(0.1)
#
#        self.tb.stop()
#        self.tb.wait()
#
#        mgs2 = dbg.get_message(0)
#        print pmt.cdr(msg)


if __name__ == '__main__':
    gr_unittest.run(qa_ccsds_encoder, "qa_ccsds_encoder.xml")
