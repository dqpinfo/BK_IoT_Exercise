import time
import json
import random
import paho.mqtt.client as mqttclient

BROKER_ADDRESS  = "app.coreiot.io"
BROKER_PORT     = 1883
ACCESS_TOKEN    = "vJXYUO4h5Xrw8bur5ios"
CLIENT_NAME     = "DHT20_02"

SUBSCRIBE_TOPIC     = "v1/devices/me/rpc/request/+"
PUBLISH_TELEMETRY   = "v1/devices/me/telemetry"

def on_connect(client, usedata, flags, rc):
    if rc == 0:
        print(f'Connected to broker: {BROKER_ADDRESS}:{BROKER_PORT}')
        client.subscribe(SUBSCRIBE_TOPIC)
    else:
        print(f'Connect to broker failed. Error code: {rc}')

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed to topic: " + SUBSCRIBE_TOPIC)

def on_message(client, userdata, message):
    print("Received: ", message.payload.decode("utf-8"))


def main():

    client = mqttclient.Client(CLIENT_NAME)
    client.username_pw_set(username = ACCESS_TOKEN, password = None)

    client.on_connect = on_connect
    client.connect(BROKER_ADDRESS, BROKER_PORT)
    client.loop_start()

    client.on_subscribe = on_subscribe
    client.on_message = on_message

    temperature = 25
    humidity = 50

    while True:

        temperature = round(temperature + random.choice([-0.1, 0.1]), 1)
        humidity    = round(humidity + random.choice([-0.1, 0.1]), 1)
        print(f'temperature: {temperature} °C- humidity: {humidity} %')

        collect_data = {'temperature': temperature, 'humidity': humidity}
        client.publish(PUBLISH_TELEMETRY, json.dumps(collect_data), 1)

        time.sleep(2)

if __name__ == '__main__':
  main()