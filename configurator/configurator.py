import re
import sys

from PyQt6 import QtWidgets, uic

from console import Console


app = QtWidgets.QApplication(sys.argv)

console = Console()
console.wait_prompt(10)

window = uic.loadUi("configurator.ui")

def send_PID():
    pitch_p = window.angle_pitch_p_box.value()
    pitch_i = window.angle_pitch_i_box.value()
    pitch_d = window.angle_pitch_d_box.value()

    roll_p = window.angle_roll_p_box.value()
    roll_i = window.angle_roll_i_box.value()
    roll_d = window.angle_roll_d_box.value()

    yaw_p = window.angle_yaw_p_box.value()
    yaw_i = window.angle_yaw_i_box.value()
    yaw_d = window.angle_yaw_d_box.value()


def read_PID():
    window.send_button.setEnabled(False)
    resp = console.send_command_with_response("config pid get angle pitch\n")
    matches = re.findall(r"=([0-9]*\.[0-9]+)", resp[0])
    window.angle_pitch_p_box.setValue(float(matches[0]))
    window.angle_pitch_d_box.setValue(float(matches[1]))
    window.angle_pitch_i_box.setValue(float(matches[2]))

    resp = console.send_command_with_response("config pid get angle roll\n")
    matches = re.findall(r"=([0-9]*\.[0-9]+)", resp[0])
    window.angle_roll_p_box.setValue(float(matches[0]))
    window.angle_roll_d_box.setValue(float(matches[1]))
    window.angle_roll_i_box.setValue(float(matches[2]))

    resp = console.send_command_with_response("config pid get angle yaw\n")
    matches = re.findall(r"=([0-9]*\.[0-9]+)", resp[0])
    window.angle_yaw_p_box.setValue(float(matches[0]))
    window.angle_yaw_d_box.setValue(float(matches[1]))
    window.angle_yaw_i_box.setValue(float(matches[2]))


window.send_button.clicked.connect(send_PID)
window.read_button.clicked.connect(read_PID)
window.send_button.setEnabled(True)
window.show()

app.exec()
