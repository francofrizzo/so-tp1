#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
  // MFQ recibe los quantums por parÃ¡metro
  // Levanto el tamaño de las pilas
	for(int i = 0; i < (int)argn.size(); i++) {
		this->def_quantum.push_back(argn[i]);
		this->vq.push_back(queue<int>());
	}
	
	// Empiezo en la cola 0
	this->n = 0;
	//asigno la cantidad de quantums
	this->quantum = this->def_quantum[0];
	// Nadie tiene un Quantum de bonificacion por bloqueo
	this->cur_pri = -1;
}

void SchedNoMistery::load(int pid) {
	// Llega un nuevo proceso lo encolo en la cola actual
	//cout << "load : " << pid << endl;
 	this->vq[this->n].push(pid);
 	this->unblock_to.push_back(this->n);
}

void SchedNoMistery::unblock(int pid) {
	//cout << "UNBLOCK : " << pid << endl;
	this->cur_pri = pid;

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
			//cout << "BLOCK : " << current_pid(cpu) << endl;
			next_pid = next();
			this->quantum = this->def_quantum[this->n];
			this->unblock_to[current_pid(cpu)] = this->n;
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
	// Si se desbloqueo un pibe y tengo q "recompensarlo"
	if (this->cur_pri != -1){
		//cout << "cur_pri: " << this->cur_pri << endl;
		this->n = this->unblock_to[this->cur_pri];
		
		next_pid = this->cur_pri ;
		this->cur_pri = -1;
	}else{
		if(!this->vq[this->n].empty()){
			// desencolo y asigno al primero
			next_pid = this->vq[this->n].front();
			this->vq[this->n].pop();
		}else{
			// busco al siguiente
			for (int i = this->n; i < this->def_quantum.size(); ++i){
				if(!this->vq[i].empty()){
					this->n = i;
					// desencolo y asigno al primero
					next_pid = this->vq[this->n].front();
					this->vq[this->n].pop();
					break;
				}
			}
			
		}
	}
	
	return next_pid;
}
