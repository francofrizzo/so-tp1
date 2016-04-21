#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	for(int i = 1; i <= argn[0]; i++){
		this->def_quantum.push_back(argn[i]);
	}

	this->cpu_quantum = this->def_quantum;
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
	this->q.push(pid);
}

void SchedRR::unblock(int pid) {
	this->q.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int next_pid = IDLE_TASK;
	if(current_pid(cpu) == IDLE_TASK){
		if(!this->q.empty()){
			next_pid = this->q.front();
			this->q.pop();
		}
	}
	else{
		switch(m){
			case TICK:
				this->cpu_quantum[cpu]--;
				if(this->cpu_quantum[cpu] == 0){
					if(!this->q.empty()){
						next_pid = this->q.front();
						this->q.pop();
						this->q.push(current_pid(cpu));
					}
					else{
						next_pid = current_pid(cpu);
					}
					this->cpu_quantum[cpu] = this->def_quantum[cpu];
				}
				else{
					next_pid = current_pid(cpu);
				}
				break;
			case BLOCK:
				if(!this->q.empty()){
					next_pid = this->q.front();
					this->q.pop();
				}
				this->cpu_quantum[cpu] = this->def_quantum[cpu];
				break;
			case EXIT:
				if(!this->q.empty()){
					next_pid = this->q.front();
					this->q.pop();
				}
				this->cpu_quantum[cpu] = this->def_quantum[cpu];
				break;
		}
	}
	return next_pid;
}
