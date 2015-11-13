
APP_DIR = /tftp
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -I/usr/local/opencv/include/opencv/ -L/usr/local/opencv/lib/ -lcxcore -lcv -lcvaux -lml -lhighgui

all:squares
	cp -f $^ $(APP_DIR)
