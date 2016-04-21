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

void TaskConsola(int pid, vector<int> params) { // params: n
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

void TaskBatch(int pid, vector<int> params) { // params: n
	int total_cpu = params[0];
	int cant_bloqueos = params[1];

	srand(time(0));

	int t = 0;
	int ultimo_t = 0;
	
	while(cant_bloqueos > 0){
		int ciclos_libres = total_cpu - (cant_bloqueos * 2);
		int ciclos_libres_norm = ciclos_libres/cant_bloqueos;
		t = rand() % (ciclos_libres_norm) ;
		ultimo_t = t + ultimo_t;
		uso_IO(pid, ultimo_t);
		total_cpu -= t;
		cant_bloqueos--;
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
}
