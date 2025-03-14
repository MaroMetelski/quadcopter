import serial
import time

class Console():
    def __init__(self, port="/dev/ttyUSB0", baud=115200):
        self.port = port
        self.baud = baud
        self.serial = serial.Serial(self.port, self.baud, timeout=0.1)
        self.prompt = "uart:~$"

    def open(self):
        if not self.serial.is_open:
            self.serial.open()

    def _writelines(self, lines):
        self.serial.writelines(lines)

    def _readlines(self):
        return self.serial.readlines()

    def wait_prompt(self, timeout):
        time_start = time.monotonic()
        while(time_start + timeout > time.monotonic()):
            self._writelines(["\n".encode()])
            lines = self._readlines()
            for line in lines:
                if self.prompt in line.decode():
                    return True
        else:
            raise TimeoutError("Failed to receive prompt")

    def send_command(self, command):
        return self._writelines([command.encode()])

    def send_command_with_response(self, command):
        self.send_command(command)
        lines = self._readlines()
        lines_decoded = []
        for line in lines[1:]:
            lines_decoded.append(line.decode())

        return lines_decoded
