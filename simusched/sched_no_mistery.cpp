#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
	cout << "SchedNoMistery"<< endl;
	// MFQ recibe los quantums por parÃ¡metro
	for(int i = 0; i < (int)argn.size(); i++) {
		// los diferentes cuantos para cada cola de prioridad
		this->def_quantum.push_back(argn[i]);
		this->vq.push_back(queue<int>());
	}
	// voy a emepezar en la cola 0
	quantum = def_quantum[0];
	n = 0;
}

void SchedNoMistery::load(int pid) {
	cout << "load"<< endl;
	this->vq[n].push(pid); // llegó una tarea nueva
}

void SchedNoMistery::unblock(int pid) {
	// se desbloqueo una tarea: La pongo a correr dsp de la tarea actual
	cout << "unblock"<< endl;
	this->vq[n].push(pid); 
	this->unblock_to[n]= pid;
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	cout << "tarea : " <<  current_pid(cpu) << endl;
	cout << "quantum : " <<  this->quantum << endl;
	bool noEsUltimaCola = this->n <( this->def_quantum.size() -1 );
	int next_pid = IDLE_TASK;
	if(current_pid(cpu) == IDLE_TASK){
		cout << "IDLE_TASK "<< endl;
		if(!this->vq[n].empty()){
			next_pid = this->vq[n].front();
			this->vq[n].pop();
			this->quantum = this->def_quantum[n];
		}else{
			if (noEsUltimaCola){
				if(!this->vq[n+1].empty()){
					// Sigo con el siguiente de la cola de menor prioridad
					this->n++;
					next_pid = this->vq[n].front();
					this->vq[n].pop();
					this->quantum = this->def_quantum[n];
				}
			}
		}
	}
	else{
		switch(m){
			case TICK:
				cout << "quantum : " <<  this->quantum << endl;
				this->quantum--;
				if (this->quantum == 0  ){
					
					// le toca al que viene
					if(!this->vq[n].empty()){
						//todavia hay pibes en la cola 
						next_pid = this->vq[n].front();
						this->vq[n].pop();
						this->quantum = this->def_quantum[n];
					}else{
						if(noEsUltimaCola){
							// me fijo en la sig cola
							if(!this->vq[n+1].empty()){
								this->n++;
								next_pid = this->vq[n].front();
								this->vq[n].pop();
								this->quantum = this->def_quantum[n];
							}else{
								// Es el único programa para correr
								next_pid = current_pid(cpu);
								this->quantum = this->def_quantum[n];
							}
						}else{
							// Es el único programa para correr
							next_pid = current_pid(cpu);
							this->quantum = this->def_quantum[n];
						}
						
					}

					// Gaste todo mi quantum
					if(noEsUltimaCola){
						// Puedo moverlo a una cola con menor prioridad
						this->vq[n+1].push(current_pid(cpu)); 
					}else{
						this->vq[n].push(current_pid(cpu));
					}

				}else{
					// me queda quantum 
					next_pid = current_pid(cpu);
				}
				break;
			case BLOCK:
				// if(!this->q.empty()){
				// 	next_pid = this->q.front();
				// 	this->q.pop();
				// }
				// this->def_quantum[cpu] = this->def_quantum[cpu];
				break;
			case EXIT:
				cout << "termino ! " << endl;
				if(!this->vq[n].empty()){
					// Sigo con el siguiente de la cola actual
					next_pid = this->vq[n].front();
					this->vq[n].pop();
					this->quantum = this->def_quantum[n];
				}else{
					if (noEsUltimaCola){
						if(!this->vq[n+1].empty()){
							// Sigo con el siguiente de la cola de menor prioridad
							this->n++;
							next_pid = this->vq[n].front();
							this->vq[n].pop();
							this->quantum = this->def_quantum[n];
						}
					}
				}
				
				break;
		}
	}
	cout << "next_pid : " <<  next_pid << endl;
	return next_pid;
}

int SchedNoMistery::next(void) {
  // Elijo el nuevo pid
}
