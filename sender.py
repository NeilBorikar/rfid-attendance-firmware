import urequests
from storage import save_json
from config import SERVER_URL, OFFLINE_FILE

def send_event(payload, offline_queue):
    try:
        r = urequests.post(SERVER_URL, json=payload)
        r.close()
        return True
    except:
        offline_queue.append(payload)
        save_json(OFFLINE_FILE, offline_queue)
        return False
