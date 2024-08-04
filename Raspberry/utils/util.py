from firebase_admin import db
import json
from utils.help import *

def initialize_data():
    try:
        with open(INIT_FIREBASE_JSON_FILE, 'r') as file:
            initData = json.load(file)
        db.reference().child("smart-home").update(initData)
    except FileNotFoundError:
        print(f"The file {INIT_FIREBASE_JSON_FILE} does not exist.")
    except json.JSONDecodeError:
        print("Error decoding JSON.")

def mode_update_json(msg):
    return {
                "light" : {
                    "balcony" : msg["data"]["light"]["balcony"],
                    "bedRoom1" : msg["data"]["light"]["bedRoom1"],
                    "bedRoom2" : msg["data"]["light"]["bedRoom2"],
                    "firstFloor" : msg["data"]["light"]["fristFloor"],
                    "garage" : msg["data"]["light"]["garage"],
                    "kitchen" : msg["data"]["light"]["kitchen"],
                    "livingRoom" : msg["data"]["light"]["livingRoom"],
                    "garageFront": msg["data"]["light"]["frontGarage"],
                    "frontDoor" : msg["data"]["light"]["frontDoor"]
                },
                "actuators": {
                    "curtain" : msg["data"]["actuator"]["curtain"],
                    "door": {
                        "balcony" : msg["data"]["actuator"]["balconyDoor"],
                        "frontDoor" : msg["data"]["actuator"]["frontDoor"],
                        "bedRoom1" : msg["data"]["actuator"]["BedRoom1Door"],
                        "bedRoom2" : msg["data"]["actuator"]["BedRoom2Door"],
                        "garage" : msg["data"]["actuator"]["garageDoor"]
                    },
                    "fan" :{
                        "bedRoom1" : msg["data"]["actuator"]["fanBed1"],
                        "bedRoom2" : msg["data"]["actuator"]["fanBed2"],
                        "kitchen" : msg["data"]["actuator"]["fanKitchen"]
                    },
                    "window" : msg["data"]["actuator"]["window"]
                }
            }

def emergency_update_json(msg):
    return {
            "actuators": {
                "curtain" : msg["data"]["curtain"],
                "door": {
                    "balcony" : msg["data"]["balconyDoor"],
                    "frontDoor" : msg["data"]["frontDoor"],
                    "bedRoom1" : msg["data"]["BedRoom1Door"],
                    "bedRoom2" : msg["data"]["BedRoom2Door"],
                    "garage" : msg["data"]["garageDoor"]
                },
                "fan" :{
                    "bedRoom1" : msg["data"]["fanBed1"],
                    "bedRoom2" : msg["data"]["fanBed2"],
                    "kitchen" : msg["data"]["fanKitchen"]
                },
                "window" : msg["data"]["window"]
            }
        }

def raining_update_json(msg):
    return {
            "actuators": {
                "curtain" : 0,
                "door": {
                    "balcony" : msg["data"]["balconyDoor"],
                    "frontDoor" : msg["data"]["frontDoor"],
                    "bedRoom1" : msg["data"]["BedRoom1Door"],
                    "bedRoom2" : msg["data"]["BedRoom2Door"],
                    "garage" : msg["data"]["garageDoor"]
                },"fan" :{
                    "bedRoom1" : msg["data"]["fanBed1"],
                    "bedRoom2" : msg["data"]["fanBed2"],
                    "kitchen" : msg["data"]["fanKitchen"]
                },
                "window" : msg["data"]["window"]
            }
        }
