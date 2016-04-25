#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
  // MFQ recibe los quantums por parÃ¡metro
  // Levanto el tamaño de las pilas
	for(int i = 1; i < (int)argn.size(); i++) {
		this->def_quantum.push_back(argn[i]);
		this->vq.push_back(queue<int>());
	}
	
	// Empiezo en la cola 0
	this->n = 0;
	//asigno la cantidad de quantums
	this->quantum = this->def_quantum[0];
}

void SchedNoMistery::load(int pid) {
	// Llega un nuevo proceso lo encolo en la cola actual
	//cout << "load : " << pid << endl;
 	this->vq[n].push(pid);
}

void SchedNoMistery::unblock(int pid) {
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	int next_pid ;
	if(current_pid(cpu) == IDLE_TASK){
		// Si no está corriendo ninguna tarea y la cola tiene elementos,
		// desencolo y asigno al primero
		next_pid = next();
	}
	else{
	// Si estaba corriendo una tarea
		if(m == TICK){
			
			// Si llega un tick, primero decremento su quantum restante
			this->quantum--;
			//cout << "tick : " << current_pid(cpu) << "quantum:" << this->quantum << endl;
			if(this->quantum == 0){
				// Si no estoy en la ultima cola 
				if(this->n < (this->def_quantum.size()-1)){
					this->vq[this->n +1].push(current_pid(cpu));
				}else{
					this->vq[this->n].push(current_pid(cpu));
				}
				next_pid = next();
				// Reinicio el quantum
				this->quantum = this->def_quantum[this->n];
			}
			else{
				// Todavia le queda quantum
				next_pid = current_pid(cpu);
			}
		}else if(m == BLOCK){
			cout << "Block" << endl;
		}else if (m == EXIT){
			// Elijo el nuevo pid
			next_pid = next();
			// Reinicio el quantum
			this->quantum = this->def_quantum[this->n];
		}
	}
	
	return next_pid;
}

int SchedNoMistery::next() {
	// Elijo el nuevo pid
	// Si no está corriendo ninguna tarea y la cola tiene elementos,
	int next_pid = IDLE_TASK;
	if(!this->vq[this->n].empty()){
		// desencolo y asigno al primero
		next_pid = this->vq[this->n].front();
		this->vq[this->n].pop();
	}else{
		// Si no estoy en la ultima cola 
		if(this->n < (this->def_quantum.size()-1)){
			// Me fijo si la siguiente cola con menor prioridad tiene elementos
			if(!this->vq[this->n + 1].empty()){
				// bajo mi prioridad
				this->n++;
				// desencolo y asigno al primero
				next_pid = this->vq[this->n].front();
				this->vq[this->n].pop();
				
			}
		}
	}
	return next_pid;
}
