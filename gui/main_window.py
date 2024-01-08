import math
from PySide6.QtWidgets import QMainWindow, QHBoxLayout, QVBoxLayout, QWidget, QGroupBox, QLabel, QPushButton, QLineEdit, QTextEdit
from PySide6.QtGui import QIcon, QPalette, QColor, QFont
from PySide6.QtCore import Qt
import pyqtgraph as pg

import serial
from threading import Thread
import time

from pyqtgraph import BarGraphItem


class MainWindow(QMainWindow):
    promotie: str = "2023-2024"
    team: list[str] = [
        "Dumitru Andreea",
        "Mocanu Razvan",
    ]

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))

        self.serial_port = None
        self.recording = False
        self.time_counter = 0.0
        
        primary_layout = QVBoxLayout()
        secondary_layout = QHBoxLayout()
        tertiary_layout = QVBoxLayout()

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member, 1)
        team_box_layout.addWidget(second_member, 1)
        team_box.setLayout(team_box_layout)

        control_panel_box = QGroupBox("Control Panel")
        control_panel_box.setFont(bold_font)

        button1 = QPushButton("Start Recording")
        button1.clicked.connect(self.start_stop_recording)
       
        button3 = QPushButton("Send")
        button3.clicked.connect(self.send_input)
        self.line_edit = QLineEdit()
        self.line_edit.setAlignment(Qt.AlignmentFlag.AlignBottom)
        line_edit_label = QLabel("COM Port:", parent=self.line_edit)
        control_panel_box_layout = QVBoxLayout()
        control_panel_box_layout.setSpacing(5)
        control_panel_box_layout.addWidget(button1, 1)

        # Create the "Switch" button
        switch_button = QPushButton("Switch")
        switch_button.clicked.connect(self.switch_action)

        # Add the "Switch" button to the control panel box layout
        control_panel_box_layout.addWidget(switch_button, 1)

        control_panel_box_layout.addStretch()
        control_panel_box_layout.addWidget(line_edit_label)
        control_panel_box_layout.addWidget(self.line_edit, 1)
        control_panel_box_layout.addWidget(button3, 1)

        control_panel_box.setLayout(control_panel_box_layout)

        tertiary_layout.addWidget(team_box, 1)
        tertiary_layout.addWidget(control_panel_box, 5)

        self.plot_widget = pg.PlotWidget()
        secondary_layout.addWidget(self.plot_widget, 3)
        secondary_layout.addLayout(tertiary_layout, 1)

        primary_layout.addLayout(secondary_layout, 4)
        self.text_edit = QTextEdit()
        self.text_edit.setReadOnly(True)

        debug_box = QGroupBox("Debug")
        debug_box_layout = QVBoxLayout()
        debug_box_layout.addWidget(self.text_edit, 1)
        debug_box.setLayout(debug_box_layout)

        primary_layout.addWidget(debug_box, 1)

        widget = QWidget()
        widget.setLayout(primary_layout)

        self.setCentralWidget(widget)
        self.bars = [
            BarGraphItem(x=[i], height=[0], width=0.5, brush=(0, 255, 0, 150)) for i in range(3)
        ]
        for bar in self.bars:
            self.plot_widget.addItem(bar)

    def start_stop_recording(self):
        if self.recording:
            self.recording = False
            self.text_edit.insertPlainText("Recording stopped.\n")
            self.serial_port.close()
        else:
            com_port = self.line_edit.text()
            
            self.serial_port = serial.Serial(port=com_port, baudrate=115200, timeout=0.5)
            self.recording = True
            self.text_edit.insertPlainText("Recording started.\n")
                # Start a thread to continuously read data from the serial port
            Thread(target=self.read_serial_data).start()
            

    def read_serial_data(self):
        min_value, max_value = float('inf'), float('-inf')

        while self.recording:
            try:
                data = self.serial_port.readline().decode("utf-8").strip()
                # Assuming data is a single numerical value received from the sensor
                value = float(data)
                db_value = value
                self.text_edit.insertPlainText(f"{self.time_counter} {db_value}\n")

                min_value = min(min_value, db_value)
                max_value = max(max_value, db_value)

                # Update the thresholds dynamically
                threshold_low_medium = min_value + (max_value - min_value) / 3
                threshold_medium_high = min_value + 2 * (max_value - min_value) / 3

                # Update the corresponding bar plot based on the value
                if db_value < threshold_low_medium:
                    self.bars[0].setOpts(x=[0], height=[db_value], brush=(0, 255, 0, 150))
                elif db_value < threshold_medium_high:
                    self.bars[1].setOpts(x=[1], height=[db_value], brush=(255, 255, 0, 150))
                else:
                    self.bars[2].setOpts(x=[2], height=[db_value], brush=(255, 0, 0, 150))

                self.time_counter += 0.5
                time.sleep(0.5)  # Adjust the sleep duration based on the sensor's update rate
            except (ValueError, UnicodeDecodeError):
                pass



    def send_input(self):
        input_data = self.line_edit.text()
        self.line_edit.clear()
        self.text_edit.insertPlainText(f"INPUT: {input_data}\n")
    
    def switch_action(self):
        if self.recording:
            # Only send the "S" value if recording is active
            try:
                # Send "S" through the serial port
                self.serial_port.write(b"S")
                self.text_edit.insertPlainText("Switch command sent.\n")
            except Exception as e:
                self.text_edit.insertPlainText(f"Error sending switch command: {e}\n")
        else:
            self.text_edit.insertPlainText("Cannot send switch command while recording is stopped.\n")