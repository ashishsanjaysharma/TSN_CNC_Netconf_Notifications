#!/bin/bash

echo "- Installing notification cnc-data yang..."
sysrepoctl -i -g ../yang/*.yang -o root:root -p 600