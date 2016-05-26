#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro

	// Guardo el quantum asociado a cada core
	for(int i = 1; i <= argn[0]; i++){
		this->def_quantum.push_back(argn[i]);
	}

	// Copio los quantums al arreglo donde se irán decrementando
	this->cpu_quantum = this->def_quantum;
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
	// Llega un nuevo proceso lo encolo
	this->q.push(pid);
}

void SchedRR::unblock(int pid) {
	// Se desbloquea un proceso lo encolo
	this->q.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int next_pid = IDLE_TASK;

	if(current_pid(cpu) == IDLE_TASK){
		// Si no está corriendo ninguna tarea y la cola tiene elementos,
		// desencolo y asigno al primero
		if(!this->q.empty()){
			next_pid = this->q.front();
			this->q.pop();
		}
	}
	else{
		// Si estaba corriendo una tarea
		if(m == TICK){
			// Si llega un tick, primero decremento su quantum restante
			this->cpu_quantum[cpu]--;
			if(this->cpu_quantum[cpu] == 0){
				// Si se quedó sin quantum y hay procesos en la cola,
				// lo desalojo
				if(!this->q.empty()){
					next_pid = this->q.front();
					this->q.pop();
					this->q.push(current_pid(cpu));
				}
				else{
					next_pid = current_pid(cpu);
				}
				// Reinicio el quantum
				this->cpu_quantum[cpu] = this->def_quantum[cpu];
			}
			else{
				next_pid = current_pid(cpu);
			}
		}
		else{
			// Si llega un EXIT o un BLOCK, desalojo la tarea
			// En caso de haber otro proceso en la cola, lo asigno
			if(!this->q.empty()){
				next_pid = this->q.front();
				this->q.pop();
			}
			// Reinicio el quantum
			this->cpu_quantum[cpu] = this->def_quantum[cpu];
		}
	}
	return next_pid;
}
