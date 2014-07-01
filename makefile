C=gcc
CFLAGS= -std=gnu99 -fPIC -Wall -Wextra -Wno-unused -I .
LDFLAGS= -std=gnu99 -L -ldl .
OBJ=NT_test.o NT_motor.o epos.o NT_pos.o NT_vel.o myCan.o NT_NMT.o NT_SDO.o NT_PDO.o printd.o  
CAN_OBJ=motor/canopen/myCan.h motor/canopen/myCan.c
OPEN_OBJ= motor/canopen/canopen.h
MOTOR_OBJ=motor/NT_motor.h motor/NT_motor.c
EPOS_OBJ=motor/epos.h motor/epos.c
VEL_OBJ=motor/NT_vel.h motor/NT_vel.c
POS_OBJ=motor/NT_pos.h motor/NT_pos.c
NMT_OBJ=motor/canopen/NT_NMT.h motor/canopen/NT_NMT.c
PDO_OBJ=motor/canopen/NT_PDO.h motor/canopen/NT_PDO.c
SDO_OBJ=motor/canopen/NT_SDO.h motor/canopen/NT_SDO.c
PRINTD_OBJ=motor/canopen/printd.h motor/canopen/printd.c
EXECUTABLE=NT_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) 
	gcc -o NT_test $(OBJ) -lntcan  
NT_motor.o : $(MOTOR_OBJ) $(EPOS_OBJ) $(OPEN_OBJ) $(NMT_OBJ) $(PDO_OBJ) 
	$(CC) $(CFLAGS) -c motor/NT_motor.c  

epos.o : $(MOTOR_OBJ) $(EPOS_OBJ) $(OPEN_OBJ)
	$(CC) $(CFLAGS) -c motor/epos.c 

NT_pos.o : $(POS_OBJ) $(MOTOR_OBJ) $(EPOS_OBJ) $(CAN_OBJ) $(PDO_OBJ)
	$(CC) $(CFLAGS) -c motor/NT_pos.c

NT_vel.o : $(VEL_OBJ) $(MOTOR_OBJ) $(EPOS_OBJ) $(PDO_OBJ) $(CAN_OBJ) $(PRINTD_OBJ) 
	$(CC) $(CFLAGS) -c motor/NT_vel.c

myCan.o : $(CAN_OBJ) $(NT_OBJ) 
	$(CC) $(CFLAGS) -c motor/canopen/myCan.c 

NT_NMT.o : $(NMT_OBJ) $(CAN_OBJ)
	$(CC) $(CFLAGS) -c motor/canopen/NT_NMT.c 

NT_SDO.o : $(SDO_OBJ) $(CAN_OBJ) $(PRINTD_OBJ)
	$(CC) $(CFLAGS) -c motor/canopen/NT_SDO.c

NT_PDO.o : $(PDO_OBJ) $(CAN_OBJ) $(PRINTD_OBJ)
	$(CC) $(CFLAGS) -c motor/canopen/NT_PDO.c
 
printd.o : $(PRINTD_OBJ)
	$(CC) $(CFLAGS) -c motor/canopen/printd.c 

.PHONY:clean
clean:
	rm -f $(EXECUTABLE) NT_test.o NT_motor.o epos.o NT_vel.o NT_pos.o myCan.o NT_NMT.o NT_PDO.o NT_SDO.o printd.o  
