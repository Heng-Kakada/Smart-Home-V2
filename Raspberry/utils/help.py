import firebase_admin
from firebase_admin import credentials
import serial

'''
    Constant Variable
'''

SMART_HOME_JSON_FILE = "json/smart-home-cred.json"
INIT_FIREBASE_JSON_FILE = "json/init.json"

FIREBASE_URL = "https://smart-home-2bc83-default-rtdb.asia-southeast1.firebasedatabase.app/"

PATH = "path"
DATA = "data"
ROOT = "root"

ROOT_PATH = "smart-home/"

ACTUATOR_PATH = '/actuators'
SENSOR_PATH = '/sensor'


ROOT_ACTUATOR_PATH = ROOT_PATH + ACTUATOR_PATH.replace('/', "")

ROOT_SENSOR_PATH = ROOT_PATH + SENSOR_PATH.replace('/', "")

LIGHT_PATH = '/light'

DHT_PATH = "/dht/"
MODE_PATH = "/mode"

DOOR_PATH = '/door'

FRONT_DOOR_PATH = DOOR_PATH + '/frontDoor'

GARAGE_DOOR_PATH = DOOR_PATH + '/garage'

'''
    Serial Object Define
'''

try:
    # mega
    sr1 = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    sr1.flush()
    #uno
    sr2 = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    sr2.flush()

except Exception:
    pass

'''
    Initialize Firebase
'''

cred = credentials.Certificate(SMART_HOME_JSON_FILE)
firebase_admin.initialize_app(cred,{
    'databaseURL' : FIREBASE_URL
})