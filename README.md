## Project TASK Description
-   Build a End Station(Netconf Server) to send the stream requirements(IEEE 802.1Qcc) to CNC(Netconf Client) as a NETCONF notification
-   Build a CNC NETCONF client to connect, subscribe and receive the notification

## Work 
-  CNC Notification written in C using sysrepo library

## Structure of the project
- The End Station(NETCONF Server) is simulated as a Docker container.
- The end-station application is packaged with CMAKE
- The CNC client is a general NETCONF client using ncclient python NETCONF library

## DockerFile Information
- The basic template is taken from https://github.com/opennetworkinglab/ODTN-emulator/blob/master/emulator-oc-lumentum/Dockerfile for the versions of 
sysrepo-netopeer2, libyang, libnetconf libraries.
- It installs the netconf related libraries, end_station application, yang module
- It starts the netconf server when docker container boots up. It is mentioned in supervisord.conf file

## End Station application Information
-  The dir "cnc_code" is the source code for end-station application
-  The dir "scripts" contains the script to install the yang module in sysrepo datastore
-  The dir "yang" contains the yang file with notification definition and a sample "station-requirements.xml" file. 
This sample xml file can be used to generate notifications. The stream requirements are needed to mention in this xml format.
- The dir "src" contains 3 files :
    - cnc_config_parser.h -- to define the structure to store the parsed data from the station-requirements.xml
    - cnc_config_parser.c -- it accepts the station-requirements.xml and parses the xml and returns the data in the form of structure
    - cnc_notifications.c -- it connects to sysrepo, takes station-requirements.xml as command line arguments, generate the notification and sends the notification

## YANG Model Information
- We need to build a YANG model with the notification definition so that the stream requirements can be sent as notificatin to CNC.
- The Keys mentioned in the JSON file are mapped to stream requirements as per TSN Types
- The YANG model(i.e the leafs) and their data types are same as IEEE 802.1Q TSN types
- The extra leafs like DeviceName, DeviceType, DeviceRole are the meta-data which are mentioned in JSON file are not part of TSN types yang 
- There are couple of leafs for the meta-data(mentioned in the JSON file) like DeviceName, DeviceType, DeviceRole are also mentioned in the YANG model

## How to Build the project

- CNC Notification 
    - Build the docker image, it will build the End station application and deploy it as "cnc_notifications" binary in /usr/bin/ 
    - To build docker image 
    ```
    docker build -t sysrepo -f Dockerfile .
    ```

## How to RUN the project

- RUN the docker image using below to start the netconf server 
    ```
    docker run -it --name sysrepo -p 830:830 --rm sysrepo:latest
    ```
    
- in another terminal, login to the docker container using below and current dir is /opt/dev
    ```
    docker exec -it sysrepo bash
    ```
- In /opt/dev, to run the end_station application to generate stream requirements
    ```
    cnc_notifications cnc_code/yang/station-requirements.xml
    ```

## In Future any changes to CNC_Notification part and how to build then
 - Some changes in source code
    ```
    cd /opt/dev && \
    cd cnc_code && mkdir build  && cd build && \
    cmake .. && \
    make -j2 && \
    make install    
    ```    