#!/usr/bin/env python
import connect_to_station
import signal
import sys

end_station_conn_params = [
                            ["localhost",830,"netconf","netconf"]
                        ]

def main():
	connect_to_station.connect_to_end_station(end_station_conn_params)

if __name__ == '__main__':
	main()
		
