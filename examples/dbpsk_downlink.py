#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: DBPSK Downlink
# Author: André Løfaldli
# GNU Radio version: v3.11.0.0git-55-g8526e6f8

from packaging.version import Version as StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import eng_notation
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
import gnuradio.ccsds as ccsds
import time
import threading



from gnuradio import qtgui

class dbpsk_downlink(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "DBPSK Downlink", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("DBPSK Downlink")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "dbpsk_downlink")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.subframes_decoded = subframes_decoded = 0
        self.sps = sps = 5
        self.frames_received = frames_received = 0
        self.frames_decoded = frames_decoded = 0
        self.data_rate = data_rate = int(2e6)
        self.variable_qtgui_label_0_1_0_0 = variable_qtgui_label_0_1_0_0 = subframes_decoded
        self.variable_qtgui_label_0_1_0 = variable_qtgui_label_0_1_0 = frames_decoded
        self.variable_qtgui_label_0_1 = variable_qtgui_label_0_1 = frames_received
        self.samp_rate = samp_rate = sps*data_rate
        self.freq = freq = int(70e6)
        self.fps = fps = 5
        self.bpsk = bpsk = digital.constellation_calcdist([-1-1j, -1+1j, 1+1j, 1-1j], [0, 1, 3, 2],
        4, 1, digital.constellation.AMPLITUDE_NORMALIZATION).base()
        self.antenna = antenna = 'RX2'

        ##################################################
        # Blocks
        ##################################################
        self.tab = Qt.QTabWidget()
        self.tab_widget_0 = Qt.QWidget()
        self.tab_layout_0 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_0)
        self.tab_grid_layout_0 = Qt.QGridLayout()
        self.tab_layout_0.addLayout(self.tab_grid_layout_0)
        self.tab.addTab(self.tab_widget_0, 'fft/const')
        self.tab_widget_1 = Qt.QWidget()
        self.tab_layout_1 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_1)
        self.tab_grid_layout_1 = Qt.QGridLayout()
        self.tab_layout_1.addLayout(self.tab_grid_layout_1)
        self.tab.addTab(self.tab_widget_1, 'stats')
        self.top_grid_layout.addWidget(self.tab, 0, 0, 1, 2)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.ccsds_decoder = ccsds.ccsds_decoder(0, True, True, True, False, False, 5, True)
        self._variable_qtgui_label_0_1_0_0_tool_bar = Qt.QToolBar(self)

        if None:
            self._variable_qtgui_label_0_1_0_0_formatter = None
        else:
            self._variable_qtgui_label_0_1_0_0_formatter = lambda x: repr(x)

        self._variable_qtgui_label_0_1_0_0_tool_bar.addWidget(Qt.QLabel("subframes decoded"))
        self._variable_qtgui_label_0_1_0_0_label = Qt.QLabel(str(self._variable_qtgui_label_0_1_0_0_formatter(self.variable_qtgui_label_0_1_0_0)))
        self._variable_qtgui_label_0_1_0_0_tool_bar.addWidget(self._variable_qtgui_label_0_1_0_0_label)
        self.tab_grid_layout_1.addWidget(self._variable_qtgui_label_0_1_0_0_tool_bar, 0, 2, 1, 1)
        for r in range(0, 1):
            self.tab_grid_layout_1.setRowStretch(r, 1)
        for c in range(2, 3):
            self.tab_grid_layout_1.setColumnStretch(c, 1)
        self._variable_qtgui_label_0_1_0_tool_bar = Qt.QToolBar(self)

        if None:
            self._variable_qtgui_label_0_1_0_formatter = None
        else:
            self._variable_qtgui_label_0_1_0_formatter = lambda x: repr(x)

        self._variable_qtgui_label_0_1_0_tool_bar.addWidget(Qt.QLabel("frames decoded"))
        self._variable_qtgui_label_0_1_0_label = Qt.QLabel(str(self._variable_qtgui_label_0_1_0_formatter(self.variable_qtgui_label_0_1_0)))
        self._variable_qtgui_label_0_1_0_tool_bar.addWidget(self._variable_qtgui_label_0_1_0_label)
        self.tab_grid_layout_1.addWidget(self._variable_qtgui_label_0_1_0_tool_bar, 0, 1, 1, 1)
        for r in range(0, 1):
            self.tab_grid_layout_1.setRowStretch(r, 1)
        for c in range(1, 2):
            self.tab_grid_layout_1.setColumnStretch(c, 1)
        self._variable_qtgui_label_0_1_tool_bar = Qt.QToolBar(self)

        if None:
            self._variable_qtgui_label_0_1_formatter = None
        else:
            self._variable_qtgui_label_0_1_formatter = lambda x: repr(x)

        self._variable_qtgui_label_0_1_tool_bar.addWidget(Qt.QLabel("frames received"))
        self._variable_qtgui_label_0_1_label = Qt.QLabel(str(self._variable_qtgui_label_0_1_formatter(self.variable_qtgui_label_0_1)))
        self._variable_qtgui_label_0_1_tool_bar.addWidget(self._variable_qtgui_label_0_1_label)
        self.tab_grid_layout_1.addWidget(self._variable_qtgui_label_0_1_tool_bar, 0, 0, 1, 1)
        for r in range(0, 1):
            self.tab_grid_layout_1.setRowStretch(r, 1)
        for c in range(0, 1):
            self.tab_grid_layout_1.setColumnStretch(c, 1)
        def _subframes_decoded_probe():
          while True:

            val = self.ccsds_decoder.num_subframes_decoded()
            try:
              try:
                self.doc.add_next_tick_callback(functools.partial(self.set_subframes_decoded,val))
              except AttributeError:
                self.set_subframes_decoded(val)
            except AttributeError:
              pass
            time.sleep(1.0 / (fps))
        _subframes_decoded_thread = threading.Thread(target=_subframes_decoded_probe)
        _subframes_decoded_thread.daemon = True
        _subframes_decoded_thread.start()
        self.qtgui_number_sink_1 = qtgui.number_sink(
            gr.sizeof_float,
            0,
            qtgui.NUM_GRAPH_HORIZ,
            1,
            None # parent
        )
        self.qtgui_number_sink_1.set_update_time(1.0/fps)
        self.qtgui_number_sink_1.set_title("")

        labels = ['doppler', '', '', '', '',
            '', '', '', '', '']
        units = ['Hz', '', '', '', '',
            '', '', '', '', '']
        colors = [("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"),
            ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black")]
        factor = [1.0*samp_rate/sps * 1.0/6.28, 1, 1, 1, 1,
            1, 1, 1, 1, 1]

        for i in range(1):
            self.qtgui_number_sink_1.set_min(i, -0.5*samp_rate/sps)
            self.qtgui_number_sink_1.set_max(i, 0.5*samp_rate/sps)
            self.qtgui_number_sink_1.set_color(i, colors[i][0], colors[i][1])
            if len(labels[i]) == 0:
                self.qtgui_number_sink_1.set_label(i, "Data {0}".format(i))
            else:
                self.qtgui_number_sink_1.set_label(i, labels[i])
            self.qtgui_number_sink_1.set_unit(i, units[i])
            self.qtgui_number_sink_1.set_factor(i, factor[i])

        self.qtgui_number_sink_1.enable_autoscale(False)
        self._qtgui_number_sink_1_win = sip.wrapinstance(self.qtgui_number_sink_1.qwidget(), Qt.QWidget)
        self.tab_layout_1.addWidget(self._qtgui_number_sink_1_win)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
            1024, #size
            window.WIN_BLACKMAN_hARRIS, #wintype
            freq, #fc
            samp_rate, #bw
            "", #name
            1,
            None # parent
        )
        self.qtgui_freq_sink_x_0.set_update_time(1.0/fps)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)
        self.qtgui_freq_sink_x_0.set_fft_window_normalized(False)

        self.qtgui_freq_sink_x_0.disable_legend()


        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "blue", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.qwidget(), Qt.QWidget)
        self.tab_grid_layout_0.addWidget(self._qtgui_freq_sink_x_0_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.tab_grid_layout_0.setRowStretch(r, 1)
        for c in range(0, 1):
            self.tab_grid_layout_0.setColumnStretch(c, 1)
        self.qtgui_const_sink_x_0 = qtgui.const_sink_c(
            1024, #size
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_const_sink_x_0.set_update_time(1.0/fps)
        self.qtgui_const_sink_x_0.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0.enable_grid(False)
        self.qtgui_const_sink_x_0.enable_axis_labels(True)

        self.qtgui_const_sink_x_0.disable_legend()

        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
            "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
            0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
            0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0.qwidget(), Qt.QWidget)
        self.tab_grid_layout_0.addWidget(self._qtgui_const_sink_x_0_win, 0, 1, 1, 1)
        for r in range(0, 1):
            self.tab_grid_layout_0.setRowStretch(r, 1)
        for c in range(1, 2):
            self.tab_grid_layout_0.setColumnStretch(c, 1)
        def _frames_received_probe():
          while True:

            val = self.ccsds_decoder.num_frames_received()
            try:
              try:
                self.doc.add_next_tick_callback(functools.partial(self.set_frames_received,val))
              except AttributeError:
                self.set_frames_received(val)
            except AttributeError:
              pass
            time.sleep(1.0 / (fps))
        _frames_received_thread = threading.Thread(target=_frames_received_probe)
        _frames_received_thread.daemon = True
        _frames_received_thread.start()
        def _frames_decoded_probe():
          while True:

            val = self.ccsds_decoder.num_frames_decoded()
            try:
              try:
                self.doc.add_next_tick_callback(functools.partial(self.set_frames_decoded,val))
              except AttributeError:
                self.set_frames_decoded(val)
            except AttributeError:
              pass
            time.sleep(1.0 / (fps))
        _frames_decoded_thread = threading.Thread(target=_frames_decoded_probe)
        _frames_decoded_thread.daemon = True
        _frames_decoded_thread.start()
        self.digital_diff_decoder_bb_0 = digital.diff_decoder_bb(2, digital.DIFF_DIFFERENTIAL)
        self.digital_costas_loop_cc_0 = digital.costas_loop_cc(6.28/100, 2, False)
        self.digital_constellation_decoder_cb_0 = digital.constellation_decoder_cb(bpsk)
        self.digital_clock_recovery_mm_xx_0 = digital.clock_recovery_mm_cc(sps, 0.25*0.175*0.175, 0.5, 0.175, 0.005)
        self.blocks_moving_average_xx_0 = blocks.moving_average_ff(1000, 1e-3, 4000, 1)
        # Create the options list
        self._antenna_options = ['RX2', 'TX/RX']
        # Create the labels list
        self._antenna_labels = ['RX2', 'TX/RX']
        # Create the combo box
        self._antenna_tool_bar = Qt.QToolBar(self)
        self._antenna_tool_bar.addWidget(Qt.QLabel("'antenna'" + ": "))
        self._antenna_combo_box = Qt.QComboBox()
        self._antenna_tool_bar.addWidget(self._antenna_combo_box)
        for _label in self._antenna_labels: self._antenna_combo_box.addItem(_label)
        self._antenna_callback = lambda i: Qt.QMetaObject.invokeMethod(self._antenna_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._antenna_options.index(i)))
        self._antenna_callback(self.antenna)
        self._antenna_combo_box.currentIndexChanged.connect(
            lambda i: self.set_antenna(self._antenna_options[i]))
        # Create the radio buttons
        self.top_layout.addWidget(self._antenna_tool_bar)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 1, 0)
        self.analog_agc2_xx_0 = analog.agc2_cc(6e-2, 1e-3, 1, 1)
        self.analog_agc2_xx_0.set_max_gain(65536)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_agc2_xx_0, 0), (self.digital_clock_recovery_mm_xx_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.analog_agc2_xx_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.blocks_moving_average_xx_0, 0), (self.qtgui_number_sink_1, 0))
        self.connect((self.digital_clock_recovery_mm_xx_0, 0), (self.digital_costas_loop_cc_0, 0))
        self.connect((self.digital_constellation_decoder_cb_0, 0), (self.digital_diff_decoder_bb_0, 0))
        self.connect((self.digital_costas_loop_cc_0, 1), (self.blocks_moving_average_xx_0, 0))
        self.connect((self.digital_costas_loop_cc_0, 0), (self.digital_constellation_decoder_cb_0, 0))
        self.connect((self.digital_costas_loop_cc_0, 0), (self.qtgui_const_sink_x_0, 0))
        self.connect((self.digital_diff_decoder_bb_0, 0), (self.ccsds_decoder, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "dbpsk_downlink")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_subframes_decoded(self):
        return self.subframes_decoded

    def set_subframes_decoded(self, subframes_decoded):
        self.subframes_decoded = subframes_decoded
        self.set_variable_qtgui_label_0_1_0_0(self.subframes_decoded)

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.set_samp_rate(self.sps*self.data_rate)
        self.digital_clock_recovery_mm_xx_0.set_omega(self.sps)

    def get_frames_received(self):
        return self.frames_received

    def set_frames_received(self, frames_received):
        self.frames_received = frames_received
        self.set_variable_qtgui_label_0_1(self.frames_received)

    def get_frames_decoded(self):
        return self.frames_decoded

    def set_frames_decoded(self, frames_decoded):
        self.frames_decoded = frames_decoded
        self.set_variable_qtgui_label_0_1_0(self.frames_decoded)

    def get_data_rate(self):
        return self.data_rate

    def set_data_rate(self, data_rate):
        self.data_rate = data_rate
        self.set_samp_rate(self.sps*self.data_rate)

    def get_variable_qtgui_label_0_1_0_0(self):
        return self.variable_qtgui_label_0_1_0_0

    def set_variable_qtgui_label_0_1_0_0(self, variable_qtgui_label_0_1_0_0):
        self.variable_qtgui_label_0_1_0_0 = variable_qtgui_label_0_1_0_0
        Qt.QMetaObject.invokeMethod(self._variable_qtgui_label_0_1_0_0_label, "setText", Qt.Q_ARG("QString", str(self._variable_qtgui_label_0_1_0_0_formatter(self.variable_qtgui_label_0_1_0_0))))

    def get_variable_qtgui_label_0_1_0(self):
        return self.variable_qtgui_label_0_1_0

    def set_variable_qtgui_label_0_1_0(self, variable_qtgui_label_0_1_0):
        self.variable_qtgui_label_0_1_0 = variable_qtgui_label_0_1_0
        Qt.QMetaObject.invokeMethod(self._variable_qtgui_label_0_1_0_label, "setText", Qt.Q_ARG("QString", str(self._variable_qtgui_label_0_1_0_formatter(self.variable_qtgui_label_0_1_0))))

    def get_variable_qtgui_label_0_1(self):
        return self.variable_qtgui_label_0_1

    def set_variable_qtgui_label_0_1(self, variable_qtgui_label_0_1):
        self.variable_qtgui_label_0_1 = variable_qtgui_label_0_1
        Qt.QMetaObject.invokeMethod(self._variable_qtgui_label_0_1_label, "setText", Qt.Q_ARG("QString", str(self._variable_qtgui_label_0_1_formatter(self.variable_qtgui_label_0_1))))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_freq_sink_x_0.set_frequency_range(self.freq, self.samp_rate)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.qtgui_freq_sink_x_0.set_frequency_range(self.freq, self.samp_rate)

    def get_fps(self):
        return self.fps

    def set_fps(self, fps):
        self.fps = fps
        self.qtgui_const_sink_x_0.set_update_time(1.0/self.fps)
        self.qtgui_freq_sink_x_0.set_update_time(1.0/self.fps)
        self.qtgui_number_sink_1.set_update_time(1.0/self.fps)

    def get_bpsk(self):
        return self.bpsk

    def set_bpsk(self, bpsk):
        self.bpsk = bpsk

    def get_antenna(self):
        return self.antenna

    def set_antenna(self, antenna):
        self.antenna = antenna
        self._antenna_callback(self.antenna)




def main(top_block_cls=dbpsk_downlink, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
