# About the project

The Bosch Bus Tracking System aims to provide users of the public transport with buses location, approximate waiting times and a way to request for a bus with wheelchair support for the physically impaired, all via a mobile app, by using an XDK on every bus, and a Raspberry Pi on every bus stop.

## How it works

The XDK uses a Thingspeak channel to upload sensor data, as well as a bus ID and a route ID, which is then retrieved by the mobile app to calculate the approximate location of the bus.

The Raspberry Pi scans the devices (XDKs) in range by using Bluetooth Low Energy (BLE), to update the buses location. The Bluetooth address is the only variable used, which eliminates the need of pairing, saving time and minimizing data loss.

## Getting started

To get started, clone the repository, and create a new project in the XDK Workbench by importing the project from the file system (excluding the Raspberry Pi folder).
