#!/usr/bin/env python
from ncclient import manager
from ncclient import operations
import xml.dom.minidom as xml_p
import xmltodict
from scheduler import run_scheduler
import threading

rpc_subscribtion_msg = '''
<rpc message-id="102" xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
    <create-subscription xmlns="urn:ietf:params:xml:ns:netconf:notification:1.0">
        <filter xmlns:cnc-data="urn:cnc:data:1.0" 
                type="xpath"  
                select="/cnc-data:cnc-notif"/>
    </create-subscription>
</rpc>
'''

def look_good_xml(notif_xml):
    print("------ RECEVIED NOTIFICATION XML DATA -----------")
    xml_data = xml_p.parseString(str(notif_xml))
    return xml_data.toprettyxml(indent=" ")

def conn_subs_thread(conn_details):
    try:
    with manager.connect(host=conn_details[0], 
        port=conn_details[1], 
        username=conn_details[2], 
        password=conn_details[3], 
        look_for_keys = False,
        hostkey_verify=False) as conn_manager:
            sess_id = conn_manager._session.id
            print("Connected to END STATION NETCONF Server with Session ID : ", sess_id)
            subs_resp = conn_manager._session.send(rpc_subscribtion_msg)
            while True:
                notification_msg = conn_manager.take_notification(block=True, timeout=None)
                notif_xml = notification_msg.notification_xml
                print(look_good_xml(notif_xml))
                print("===== CALLING SCHEDULER ==========")
                
                    
    except:
        print("END STATION DEVICE NOT REACHABLE")



def connect_to_end_station(end_station_conn_details):
    threads = list()
    for end_station_conn in range(len(end_station_conn_details)):
        conn_threads = threading.Thread(target=conn_subs_thread, args=(end_station_conn_details[end_station_conn],))
        #conn_threads.daemon = True
        threads.append(conn_threads)
        conn_threads.start()
        #print end_station_conn_params[end_station_conn]




'''
def connect_to_end_station(end_station_conn_details):
    for end_station_conn in range(len(end_station_conn_details)):
        #print end_station_conn_params[end_station_conn]
        conn_details =  end_station_conn_details[end_station_conn]
        with manager.connect(host=conn_details[0], 
            port=conn_details[1], 
            username=conn_details[2], 
            password=conn_details[3], 
            hostkey_verify=False,
            look_for_keys = False) as conn_manager:
                conn_manager._session.send(rpc_subscribtion_msg)
                while True:
                    notification_msg = conn_manager.take_notification(block=True, timeout=None)
                    notif_xml = notification_msg.notification_xml
                    print look_good_xml(notif_xml)
                    run_scheduler(notif_xml)
'''
            
