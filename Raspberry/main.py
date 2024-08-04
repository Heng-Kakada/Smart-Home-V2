import threading
import json
from firebase_admin import db

from utils.util import initialize_data
from utils.transmit_action import action_on_sr1, action_on_sr2
from utils.help import *

from server import app

'''
    When we initialize_data function work. It gonna go to firebase for 
    updating all value throw defualt.
'''

initialize_data()




def transmitting():

    '''
        transmitting function It work when something send from
        our serial port then update to cloud (firebase).
    '''

    while True:
        try:            
            if sr1.in_waiting > 0:
                line =  sr1.readline().decode('utf-8').strip()
                
                print(f"Transmit => {line}")
                
                msg = json.loads(line)

                thread_sr1 = threading.Thread(target=action_on_sr1, daemon=True, args=(msg,))
                thread_sr1.start()
                
            if sr2.in_waiting > 0:
                line =  sr2.readline().decode('utf-8').strip()

                print(f"Transmit => {line}")
                
                msg = json.loads(line)

                thread_sr2 = threading.Thread(target=action_on_sr2, daemon=True, args=(msg,))
                thread_sr2.start()

        except json.JSONDecodeError as e:
            print(f"JSON Decode Error Trainsmtting > {e}")
        except Exception as e:
            print(f"Error Trainsmtting > {e}")




def callBackHanlder(event):

    '''
        this callBack function happen when something change at cloud (firebase).
        Example : I click turn on bacony light on our mobile app.Then it will go to cloud 
        update on path bacony light and set it to `1`.When cloud has something to change like
        that our callBack function will fetch data and send to a serial port.The data that 
        it send to serial as byte of json.
    '''


    try:
        if LIGHT_PATH in event.path or ACTUATOR_PATH in event.path :
            
            path = event.path.split("/",2)
            dic = {
                ROOT : path[1],
                PATH : path[2],
                DATA : event.data
            }
            data = json.dumps(dic)

            sr1.write(bytes(data, 'utf-8'))

            print(f"Recieve => {data}")

        elif MODE_PATH in event.path:
            dic = {
                ROOT : MODE_PATH.replace('/', ''),
                DATA : event.data
            }
            data = json.dumps(dic)

            sr1.write(bytes(data, 'utf-8'))

            print(f"Recieve => {data}")

    except Exception as e:
            print(f"Error Recieving > {e}")
def recieving():
    db.reference("/smart-home/").listen(callBackHanlder)


# threading
transmit_thread = threading.Thread(target=transmitting, daemon=True)
recieve_thread = threading.Thread(target=recieving, daemon=True)
#server_thread = threading.Thread(target=app.run_server, daemon=True)

transmit_thread.start()
recieve_thread.start()
#server_thread.start()

print("Server Running...")

# Keep the main thread alive
try:
    while True:
        pass
except KeyboardInterrupt:
    print("Shutting down...")