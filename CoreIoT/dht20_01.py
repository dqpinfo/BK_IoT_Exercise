"""
https://app.coreiot.io
Entities => Devices (All) => Device details => Manage credentials => MQTT Basic
Access token: egkdsm5c50w26x6qrtsn

Entities => Devices (Groups)
Add entity group => Name: Public Devices => Add => Make public
All: Devices => Select devices => Add to group => Select existing entity group => Public Devices => Add

Entities => Devices (All) => Device details => Latest telemetry

Library: paho-mqtt v1.6.1
"""

import time
import json
import paho.mqtt.client as mqttclient

BROKER_ADDRESS  = "app.coreiot.io"
BROKER_PORT     = 1883
ACCESS_TOKEN    = "egkdsm5c50w26x6qrtsn"
CLIENT_NAME     = "DHT20_01"

SUBSCRIBE_TOPIC = "v1/devices/me/rpc/request/+"
PUBLISH_TOPIC   = "v1/devices/me/telemetry"

def on_connect(client, usedata, flags, rc):
    if rc == 0:
        print("Connect to broker successfully !!!")

        client.subscribe(SUBSCRIBE_TOPIC)
    else:
        print(f'Connect to broker failed. Error code: {rc}')

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribe to topic successfully: " + SUBSCRIBE_TOPIC)

def on_message(client, userdata, message):
    
    print("Received: ", message.payload.decode("utf-8"))

    try:
        json_obj = json.loads(message.payload)

        if json_obj['method'] == "setValue":

            temp_data = {'value': True}
            temp_data['value'] = json_obj['params']

            print("temp_data: ", temp_data)
            #client.publish('v1/devices/me/attributes', json.dumps(temp_data), 1)
    except:
        pass


def main():

    client = mqttclient.Client(CLIENT_NAME)
    client.username_pw_set(username = ACCESS_TOKEN, password = None)

    client.on_connect = on_connect
    client.connect(BROKER_ADDRESS, BROKER_PORT)
    client.loop_start()

    client.on_subscribe = on_subscribe
    client.on_message = on_message

    temperature = 0
    humidity = 10

    while True:

        temperature += 1
        humidity += 1

        collect_data = {'temperature': temperature, 'humidity': humidity}
        client.publish(PUBLISH_TOPIC, json.dumps(collect_data), 1)

        time.sleep(2)

if __name__ == '__main__':
  main()