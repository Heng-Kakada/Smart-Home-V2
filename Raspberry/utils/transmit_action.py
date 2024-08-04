from firebase_admin import db

from utils.help import *
from utils.util import mode_update_json, raining_update_json, emergency_update_json

def action_on_sr1(msg):
    if msg[PATH] == "dht" :
        db.reference().child(ROOT_SENSOR_PATH + DHT_PATH).update({
            "temp" : msg[DATA]["temp"],
            "hum" : msg[DATA]["hum"]
        })
    elif msg[PATH] == "flame":
        db.reference().child(ROOT_SENSOR_PATH).update({ "flame" : msg[DATA] })
    elif msg[PATH] == "gas":
        db.reference().child(ROOT_SENSOR_PATH).update({ "gas" : msg[DATA] })
    elif msg[PATH] == "rain":
        db.reference().child(ROOT_SENSOR_PATH).update({ "rain" : msg[DATA] })
    elif msg[PATH] == "car1":
        db.reference().child(ROOT_SENSOR_PATH).update({ "car1" : msg[DATA] })
    elif msg[PATH] == "car2":
        db.reference().child(ROOT_SENSOR_PATH).update({ "car2" : msg[DATA] })
    elif msg[PATH] == "mode":
        db.reference().child(ROOT_PATH).update(mode_update_json(msg))
    elif msg[PATH] == "emergency":
        db.reference().child(ROOT_PATH).update(emergency_update_json(msg))
    elif msg[PATH] == "raining":
        db.reference().child(ROOT_PATH).update(raining_update_json(msg))
    elif msg[PATH] == 'frontDoor':
        db.reference().child( ROOT_ACTUATOR_PATH + FRONT_DOOR_PATH ).set(msg[DATA])

def action_on_sr2(msg):
    if msg[PATH] == "rfid":
        db.reference().child( ROOT_ACTUATOR_PATH + GARAGE_DOOR_PATH ).set(msg[DATA])