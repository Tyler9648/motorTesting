TARGET = main

OBJ_C = $(wildcard *.c )
OBJ_O = $(patsubst %.c, %.o,$(notdir ${OBJ_C}))

#BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc

DEBUG = -g -O0 -I.
CFLAGS += $(DEBUG)

# USELIB = USE_BCM2835_LIB
# USELIB = USE_WIRINGPI_LIB
# USELIB = USE_DEV_LIB
DEBUG = -D #$(USELIB) 
#ifeq ($(USELIB), USE_BCM2835_LIB)
#    LIB = -lbcm2835 -lm 
#else ifeq ($(USELIB), USE_WIRINGPI_LIB)
#    LIB = -lwiringPi -lm 
#
#endif

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB) -lm -lpigpio

%.o : %.c
	$(CC) $(CFLAGS) -c  $< -o $@

clean:
	rm *.o 
	rm $(TARGET) 

run: $(TARGET)
		./$(TARGET) $(TESTOPTIONS)