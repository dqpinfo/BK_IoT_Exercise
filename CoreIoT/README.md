# CoreIoT
Link: https://app.coreiot.io

# Devices
Entities -> Devices (All) -> Device details -> Manage credentials -> Access token  
Entities -> Devices (Groups) -> Add entity group -> Name: Public Devices -> Add -> Make public  
All: Devices -> Select devices -> Add to group -> Select existing entity group -> Public Devices -> Add  
Entities -> Devices (All) -> Device details -> Latest telemetry

# Dashboards
Dashboards -> Add dashboard -> Create new dashboard -> Basic Dashboard -> Add  

Add widget -> Cards -> Simple Value and chart card -> Add  
Datasource  
Device:     DHT20_01  
Data key:   temperature

Add widget -> Control widgets -> Switch Control -> Add

Add widget -> Maps -> OpenStreet Map -> Add  
**Data**  
Type:       Device  
Device:     DHT20_01  
Data keys:  latitude, longitude  
**Appearance**  
Map provider setting: Google maps  
Latitude key name:    latitude  
Longitude key name:   longitude

# Chạy chương trình
1. Tạo và active môi trường ảo:
python -m venv .venv
source .venv/bin/activate

2. Chọn interpreter bên trong thư mục .venv
Cmd + Shift + P
Python: Select Interpreter

3. Cài đặt các thư viện sử dụng (paho-mqtt v1.6.1)
pip install -r requirements.txt

4. Chạy DHT simulator
Terminal 1: python dht20_01.py
Terminal 2: python dht20_02.py
