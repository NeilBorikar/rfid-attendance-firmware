from datetime import datetime
from sqlalchemy.orm import Session

from repositories.attendance_repository import AttendanceRepository
from repositories.device_repository import DeviceRepository
from repositories.rfid_repository import RFIDRepository
from services.notification_service import NotificationService

# =========================
# CONFIGURATION
# =========================
SSID = "YOUR_WIFI"
PASSWORD = "YOUR_PASSWORD"

SERVER_URL = "https://your-server.com/attendance" #URL Name
DEVICE_ID = "SCHOOL_GATE_1" 

STATE_FILE = "attendance_state.json" #JSON file name
OFFLINE_FILE = "offline_queue.json" #JSON file name

# =========================
# LOAD / SAVE STATE
# =========================
def load_json(filename, default):
    try:
        with open(filename, "r") as f:
            return ujson.load(f)
    except:
        return default

def save_json(filename, data):
    with open(filename, "w") as f:
        ujson.dump(data, f)

last_status = load_json(STATE_FILE, {})
offline_queue = load_json(OFFLINE_FILE, [])

# =========================
# WIFI CONNECTION
# =========================
def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    if not wlan.isconnected():
        print("Connecting to WiFi...")
        wlan.connect(SSID, PASSWORD)
        while not wlan.isconnected():
            time.sleep(1)

    print("WiFi Connected:", wlan.ifconfig())

connect_wifi()

# =========================
# RFID SETUP
# =========================
spi = SPI(
    2,
    baudrate=2500000,
    polarity=0,
    phase=0,
    sck=Pin(18),
    mosi=Pin(23),
    miso=Pin(19)
)

rfid = MFRC522(spi=spi, gpioRst=22, gpioCs=5)

print("RFID Attendance System Ready")

# =========================
# RESEND OFFLINE DATA
# =========================
def resend_offline_data():
    global offline_queue

    if not offline_queue:
        return

    print("Resending offline records...")

    remaining = []

    for record in offline_queue:
        try:
            r = urequests.post(SERVER_URL, json=record)
            r.close()
            print("Resent:", record)
        except:
            remaining.append(record)

    offline_queue = remaining
    save_json(OFFLINE_FILE, offline_queue)

# =========================
# MAIN LOOP
# =========================
while True:
    (status, _) = rfid.request(rfid.REQIDL)

    if status == rfid.OK:
        (status, uid) = rfid.anticoll()

        if status == rfid.OK:
            card_uid = "-".join("{:02X}".format(x) for x in uid)

            # -------- IN / OUT LOGIC --------
            if card_uid not in last_status:
                current_status = "IN"
            elif last_status[card_uid] == "IN":
                current_status = "OUT"
            else:
                current_status = "IN"

            last_status[card_uid] = current_status
            save_json(STATE_FILE, last_status)

            print("Card:", card_uid, "Status:", current_status)

            payload = {
                "uid": card_uid,
                "status": current_status,
                "device_id": DEVICE_ID,
                "timestamp": time.time()
            }

            try:
                r = urequests.post(SERVER_URL, json=payload)
                r.close()
                print("Sent to server")

                resend_offline_data()

            except:
                print("Offline – saving locally")
                offline_queue.append(payload)
                save_json(OFFLINE_FILE, offline_queue)

            time.sleep(3)
