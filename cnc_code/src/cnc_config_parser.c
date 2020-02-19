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

endStationRequirements 
cnc_config_parse(const char *config_file)
{
    endStationRequirements end_stations;
    //const char *config_file;
    printf("config file %s\n", config_file);
    struct lyxml_elem *xml = NULL;
    struct ly_ctx *ctx = NULL;
    ctx = ly_ctx_new(NULL, 0);
    xml = lyxml_parse_path(ctx, config_file, 0);
    struct lyxml_elem *child_element = xml->child;
    
    while(child_element)
    {
	struct lyxml_elem *DeviceName = child_element->child;
	end_stations.DeviceName = strdup(DeviceName->content);
	struct lyxml_elem *DeviceType = DeviceName->next;
	end_stations.DeviceType = strdup(DeviceType->content);
	struct lyxml_elem *Period = DeviceType->next;
	end_stations.Period = atoi(Period->content);
	struct lyxml_elem *StreamID = Period->next;
	end_stations.StreamID = strdup(StreamID->content);
	struct lyxml_elem *StreamAddress = StreamID->next;
	end_stations.StreamAddress = strdup(StreamAddress->content);
	struct lyxml_elem *MaxFrameSize = StreamAddress->next;
	end_stations.MaxFrameSize = atoi(MaxFrameSize->content); //atoi to convert string into integer
	struct lyxml_elem *VlanId = MaxFrameSize->next;
	end_stations.VlanId = atoi(VlanId->content);
	struct lyxml_elem *Priority = VlanId->next;
    end_stations.Priority = atoi(Priority->content);
	struct lyxml_elem *DeviceRole = Priority->next;
	end_stations.DeviceRole = strdup(DeviceRole->content);
	struct lyxml_elem *interface_subchild = DeviceRole->next;
	while(interface_subchild)
    	{
		struct lyxml_elem *PortName = interface_subchild->child;
		end_stations.interface.PortName = strdup(PortName->content);
		struct lyxml_elem *WindowStart = PortName->next;
		end_stations.interface.WindowStart = atoi(WindowStart->content);
		struct lyxml_elem *WindowEnd = WindowStart->next;
		end_stations.interface.WindowEnd = atoi(WindowEnd->content);
		interface_subchild=interface_subchild->next;
   	}
	child_element=child_element->next;
    }
   lyxml_free(ctx, xml);

return end_stations;
	
}


