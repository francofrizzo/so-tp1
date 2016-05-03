#include "tasks.h"


using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params) { // params: n, bmin, bmax
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	srand(time(0));

	int t;
	for(int i = 0; i < n; i++){
		t = rand() % (bmax - bmin + 1) + bmin;
		uso_IO(pid, t);
	}
}

void TaskIORandom(int pid, vector<int> params) { // params: ms_cpu, bmin, bmax
	int bmin = params[1];
	int bmax = params[2];
	srand(time(0));

	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.

	int t = rand() % (bmax - bmin + 1) + bmin;
	uso_IO(pid, t);
}

void TaskBatch(int pid, vector<int> params) { // params: n
	int cant_bloqueos = params[1];
	int total_cpu = params[0] - cant_bloqueos - 1;

	srand(time(0));

	vector<int> ints;	
		
    int tInt;
    for (int i = 0; i < cant_bloqueos; ++i){
	    do{
			tInt = rand() % total_cpu;
		}while(find(ints.begin(), ints.end(), tInt) != ints.end());
		ints.push_back(tInt);
	}

	sort(ints.begin(), ints.end());
	for (int i = 0; i < cant_bloqueos; ++i){
		int cpuBurst = i == 0 ? ints[i] : ints[i] - ints[i-1];
		uso_CPU(pid,cpuBurst);
		uso_IO(pid,2);
	}
	uso_CPU(pid,total_cpu - ints[cant_bloqueos - 1] );
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskIORandom, 3);
	register_task(TaskBatch, 2);
}
