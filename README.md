Traffic Packet
==============
Using cars as routers

Overview
--------
As autonomous vehicles become more prominent, it becomes important for vehicles to communicate between each other within a city via a peer-to-peer mobile protocol. Due to the limited transmission range and variable mobility of these vehicles, protocols need to be developed to efficiently transfer data between any number of vehicles to reach the intended destination. In this project, we will create a simulation environment to simulate data transfers between multiple vehicle types within a grid style city. Using the city walk model, we will simulate a grid style city with multiple vehicle types each acting as a node point to transmit data within our network. 

Goals
-----
1. Have a robust simulation which vehicles create a peer-to-peer network for transmitting data
2. Create an environment with different vehicle types with a variety of navigation and transmission options
3. Transport packets in a mobile routing network

Specifications
-------------
* Simulation implemented and compiled in a *nix environment

Roles
-----
* Brandon - Develop simulator kernel
* Daniel - Implement routing protocol
* Tyler - Design nodal behavior
  
Milestones
----------
1. Develop a simulated city environment populated with vehicles using a city walk model
2. Transport from one stationary port to another stationary port using the vehicles as transmission nodes
3. Transmit packets from a moving vehicle to a stationary port
4. Decrease round-trip-time from a moving vehicle to a stationary port

Build and Dependencies
----------------------
This build uses *CMake* to compile. Please set your directory to the ```traffic-packet``` repository directory before continuing with instructions.

## Make (default) ##
```bash
mkdir build
cd build
cmake ..
make
./DTN
```

## Xcode ##
```bash
mkdir build
cd build
cmake -G Xcode ..
open -a Xcode DTN.xcodeproj
```
