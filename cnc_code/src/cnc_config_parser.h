#ifndef CNC_CONFIG_PARSER_H_
#define CNC_CONFIG_PARSER_H_

struct INTERFACES{
	char* PortName;
	int WindowStart;
	int WindowEnd;
};

typedef struct{
	char*        	DeviceName;
	char*           DeviceType;
	int           	Period;
	char*           StreamID;
	char*           StreamAddress;
	int           	MaxFrameSize;
	int           	VlanId;
	int           	Priority;
	char*           DeviceRole;
	struct         	INTERFACES interface;
} endStationRequirements;

endStationRequirements cnc_config_parse(const char *config_file);

#endif
