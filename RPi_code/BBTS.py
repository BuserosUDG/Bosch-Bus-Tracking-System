# Bluetooth modules
import bluepy
from bluepy import btle
from bluepy.btle import Scanner
# HTTP modules
import urllib2
# JSON parser
import json
#System module
import os
import time

bus_stop_ID = "N01"

## Only the last address is valid. The first two act as dummy addresses
## to be compared against.
bus_addresses = ["fc:d6:bd:10:0f:2d","fc:d6:bd:10:0f:2c","fc:d6:bd:10:0f:2b"]
buses = ["B01","B02","B03"]
routes = ["R622","R622","R622"]
special_service = ['Y','N','N']

arrived_bus_ID = ''
arrived_bus_route = ''

myAPI = '82YNHFSEJ8PNTZ52'
baseURL = 'https://api.thingspeak.com/update?api_key=%s' %myAPI

buffer=[]
index_w = 0
index_r = 0
counter_var = 0
for k in range(0,20):
    buffer.append(0)

## Checks for root access, needed for bluetooth
euid = os.geteuid()
if euid != 0:
    raise EnvironmentError, "need to be root"
    exit()
                    
##BLE scan
def ble_scan():
    global devices
    scanner = Scanner()
    print "Searching for device..."
    devices = scanner.scan(3.0)

## Prints valid XDK devices in range
def ble_find(XDK_Bus):
    global XDK_found
    for dev in devices:
        if XDK_Bus == dev.addr:
            XDK_found = dev.addr
            print "XDK found with address", dev.addr
            return 1
    return 0

## Sends bus position to Thingspeak	
def sends_data(ind):
    arrived_bus_ID = buses[ind]
    arrived_bus_route = routes[ind]
    servicio_e = special_service[ind]
    bus_route_stop_id = arrived_bus_ID+servicio_e+"-"+arrived_bus_route+"-"+bus_stop_ID
    conn = urllib2.urlopen(baseURL+'&field1=%s'%bus_route_stop_id)
    print "POST no:" + conn.read()
    conn.close()
    print bus_route_stop_id

## Main program	
while True:
    ble_scan()
    for x in range(0, len(bus_addresses)):
        if ble_find(bus_addresses[x]):
            buffer[index_w] = x
            index_w+=1
            if index_w == 20:
                index_w = 0
        x+=1
    if counter_var == 4:
        counter_var = 0
        if index_r != index_w:
            sends_data(buffer[index_r])
            index_r+=1
            if index_r == 20:
                index_r = 0
    else:
        counter_var+=1
    print "i_w:", index_w
    print "i_r:", index_r
            
            
            

        

