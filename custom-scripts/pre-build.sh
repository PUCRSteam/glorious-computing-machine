#!/bin/sh

# exporting path to get access to the cross compiler
export PATH=$PATH:$BASE_DIR/../output/host/bin

# copying file to configure network at startup
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

# setting up the startup message
i686-linux-gcc $BASE_DIR/../custom-scripts/startupMessage.c -O2 -o $BASE_DIR/../output/target/usr/bin/startupMessage

# copying the system monitor script
cp $BASE_DIR/../custom-scripts/httpServer/monitor.sh $BASE_DIR/target/usr/bin
chmod +x $BASE_DIR/target/usr/bin/monitor.sh

# setting up the httpServer for the system monitor
i686-linux-gcc $BASE_DIR/../custom-scripts/httpServer/httpServer.c -O2 -o $BASE_DIR/../output/target/usr/bin/httpServer
