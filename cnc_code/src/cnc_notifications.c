#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inttypes.h>
#include <math.h>
#include "sysrepo.h"
#include "sysrepo/values.h"
#include "libyang/libyang.h"
#include <string.h>
#include "cnc_config_parser.h"

volatile int exit_application = 0;

static int send_cnc_notification(sr_session_ctx_t *sess, const char *config_file)
{
	int rc;
	sr_val_t *cnc_notification; //structure to hold sysrepo data values
	size_t array_xml_values= 0; //array index to hold array values of parsed input xml info
	
	endStationRequirements stations_data = cnc_config_parse(config_file);
	
    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
	sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/DeviceName");
	sr_val_set_str_data(&cnc_notification[array_xml_values - 1], SR_STRING_T, stations_data.DeviceName);

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
	sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/DeviceType");
	sr_val_set_str_data(&cnc_notification[array_xml_values - 1], SR_STRING_T, stations_data.DeviceType);

	sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/max-latency");
	cnc_notification[array_xml_values - 1].type = SR_UINT32_T;
    cnc_notification[array_xml_values - 1].data.uint32_val = stations_data.Period;


	sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/stream-id-type");
	sr_val_set_str_data(&cnc_notification[array_xml_values - 1], SR_STRING_T, stations_data.StreamID);

	sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/mac-address");
    sr_val_set_str_data(&cnc_notification[array_xml_values - 1], SR_STRING_T, stations_data.StreamAddress);

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/max-frame-size");
    cnc_notification[array_xml_values - 1].type = SR_UINT16_T;
    cnc_notification[array_xml_values - 1].data.uint16_val = stations_data.MaxFrameSize;

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/vlan-id");
    cnc_notification[array_xml_values - 1].type = SR_UINT16_T;
    cnc_notification[array_xml_values - 1].data.uint16_val = stations_data.VlanId;

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/priority-code-point");
    cnc_notification[array_xml_values - 1].type = SR_UINT8_T;
    cnc_notification[array_xml_values - 1].data.uint8_val = stations_data.Priority;

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s", "/cnc-data:cnc-notif/DeviceRole");
    sr_val_set_str_data(&cnc_notification[array_xml_values - 1], SR_STRING_T, stations_data.DeviceRole);
    
    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s[interface-name='%s']/%s", "/cnc-data:cnc-notif/interface", stations_data.interface.PortName, "interface-name");
    cnc_notification[array_xml_values - 1].type = SR_STRING_T;
    cnc_notification[array_xml_values - 1].data.string_val = stations_data.interface.PortName;

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s[interface-name='%s']/%s", "/cnc-data:cnc-notif/interface", stations_data.interface.PortName, "earliest-transmit-offset");
    cnc_notification[array_xml_values - 1].type = SR_UINT32_T;
    cnc_notification[array_xml_values - 1].data.uint32_val = stations_data.interface.WindowStart;

    sr_realloc_values(array_xml_values, array_xml_values+1, &cnc_notification);
    array_xml_values++;
    sr_val_build_xpath(&cnc_notification[array_xml_values - 1], "%s[interface-name='%s']/%s", "/cnc-data:cnc-notif/interface", stations_data.interface.PortName, "latest-transmit-offset");
    cnc_notification[array_xml_values - 1].type = SR_UINT32_T;
    cnc_notification[array_xml_values - 1].data.uint32_val = stations_data.interface.WindowEnd;

	rc = sr_event_notif_send(sess, "/cnc-data:cnc-notif", cnc_notification, array_xml_values, SR_EV_NOTIF_DEFAULT);
	if (rc != SR_ERR_OK) {
		printf("Failed to send notification\n");
		return SR_ERR_OPERATION_FAILED;
	}

	printf("Successfully sent endstation requirements notification to CNC \n");

	sr_free_values(cnc_notification, array_xml_values);

	return SR_ERR_OK;
}

int
main(int argc, char **argv)
{
    /*connection, session and subscription parameters intialization */
    sr_conn_ctx_t *connection = NULL;
    sr_session_ctx_t *session = NULL;
    sr_subscription_ctx_t *subscription = NULL;
    int rc = SR_ERR_OK;
    setbuf(stdout, NULL);

    /*Taking stream requirements as a XML file through command line arguments */
    const char *config_file;
    if (argc > 1) {
        config_file = argv[1];
    }
  
    /* connect to sysrepo datastore */
    rc = sr_connect("cnc-data", SR_CONN_DEFAULT, &connection);
    if (SR_ERR_OK != rc) {
        fprintf(stderr, "ERROR CONNECTING TO SYSREPO: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    /* start session with sysrepo datastore */
    rc = sr_session_start(connection, SR_DS_RUNNING, SR_SESS_DEFAULT, &session);
    if (SR_ERR_OK != rc) {
        fprintf(stderr, "ERROR IN STARTING SESSION WITH SYSREPO: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    /* send notification to subscribed CNC NETCONF CLIENT */
    send_cnc_notification(session, config_file);
    
cleanup:
    if (NULL != subscription) {
        sr_unsubscribe(session, subscription);
    }
    if (NULL != session) {
        sr_session_stop(session);
    }
    if (NULL != connection) {
        sr_disconnect(connection);
    }
    return rc;
}
