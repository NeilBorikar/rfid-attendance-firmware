from machine import Pin, SPI
from mfrc522 import MFRC522

def setup_rfid():
    spi = SPI(
        2,
        baudrate=2500000,
        polarity=0,
        phase=0,
        sck=Pin(18),
        mosi=Pin(23),
        miso=Pin(19)
    )

    return MFRC522(spi=spi, gpioRst=22, gpioCs=5)

def read_card(rfid):
    (status, _) = rfid.request(rfid.REQIDL)
    if status != rfid.OK:
        return None

    (status, uid) = rfid.anticoll()
    if status != rfid.OK:
        return None

    return "-".join("{:02X}".format(x) for x in uid)
