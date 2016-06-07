export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

ejercicio1:
	./simusched tasks/ej1.tsk 1 1 1 SchedFCFS | python graphsched.py > ../latex/imagenes/ej1.png

ejercicio2:
	./simusched tasks/ej2.tsk 1 5 1 SchedFCFS | python graphsched.py > ../latex/imagenes/ej2_1.png
	./simusched tasks/ej2.tsk 2 5 1 SchedFCFS | python graphsched.py > ../latex/imagenes/ej2_2.png
	./simusched tasks/ej2.tsk 4 5 1 SchedFCFS | python graphsched.py > ../latex/imagenes/ej2_3.png

ejercicio3:
	./simusched tasks/ej3.tsk 1 1 1 SchedFCFS | python graphsched.py > ../latex/imagenes/ej3.png

ejercicio4:
	./simusched tasks/ej4.tsk 2 1 1 SchedRR 5 10 | python graphsched.py > ../latex/imagenes/ej4.png

ejercicio5:
	./simusched tasks/ej5.tsk 3 1 1 SchedRR 2 10 30 | python graphsched.py > ../latex/imagenes/ej5.png
	./simusched tasks/ej5.tsk 1 2 1 SchedRR 2 | python graphsched.py > ../latex/imagenes/ej5_q2.png
	./simusched tasks/ej5.tsk 1 2 1 SchedRR 10 | python graphsched.py > ../latex/imagenes/ej5_q10.png
	./simusched tasks/ej5.tsk 1 2 1 SchedRR 30 | python graphsched.py > ../latex/imagenes/ej5_q30.png

ejercicio6:
	./simusched tasks/ej5.tsk 1 2 1 SchedFCFS | python graphsched.py > ../latex/imagenes/ej6.png

ejercicio7:
	./simusched tasks/ej7_1.tsk 1 1 1 SchedMistery 5 10 | python graphsched.py > ../latex/imagenes/ej7_1.png
	./simusched tasks/ej7_2.tsk 1 1 1 SchedMistery 5 10 | python graphsched.py > ../latex/imagenes/ej7_2.png
	./simusched tasks/ej7_3.tsk 1 1 1 SchedMistery 5 10 | python graphsched.py > ../latex/imagenes/ej7_3.png

ejercicio8:
	./simusched tasks/ej8_1.tsk 2 1 6 SchedRR 8 8 | python graphsched.py > ../latex/imagenes/ej8_1_rr.png
	./simusched tasks/ej8_1.tsk 2 1 6 SchedRR2 8 8 | python graphsched.py > ../latex/imagenes/ej8_1_rr2.png
	./simusched tasks/ej8_2.tsk 2 1 4 SchedRR 12 4 | python graphsched.py > ../latex/imagenes/ej8_2_rr.png
	./simusched tasks/ej8_2.tsk 2 1 4 SchedRR2 12 4 | python graphsched.py > ../latex/imagenes/ej8_2_rr2.png
	./simusched tasks/ej8_3.tsk 2 1 4 SchedRR 8 8 | python graphsched.py > ../latex/imagenes/ej8_3_rr.png
	./simusched tasks/ej8_3.tsk 2 1 4 SchedRR2 8 8 | python graphsched.py > ../latex/imagenes/ej8_3_rr2.png

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all
