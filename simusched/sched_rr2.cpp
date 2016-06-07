#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro

    ncores = argn[0];  // guardo la cantidad de cores

    // Inicialización de estructuras
    q           = std::vector< std::queue<int> *>(ncores);
    tasks       = std::vector< std::vector<int> *>(ncores);
    cpu_quantum = std::vector<int>(ncores);
    def_quantum = std::vector<int>(ncores);
    proc_count  = std::vector<int>(ncores, 0);

    for (int i = 0; i < ncores; i++) {
        q[i]     = new std::queue<int>;  // cola asociada al core
        tasks[i] = new std::vector<int>; // vector de tareas asociadas al core

        int quantum_size = argn[i + 1];  // tiempo de quantum del core
        cpu_quantum[i] = quantum_size;   // este se va a ir decrementando
        def_quantum[i] = quantum_size;   // este queda fijo
    }
}

SchedRR2::~SchedRR2() {
    for (int i = 0; i < ncores; i++) {
        delete q[i];
        delete tasks[i];
    }
}


void SchedRR2::load(int pid) {
    // Decido a qué core asignar el proceso
    int sel_cpu = min_index(proc_count);
    // Lo encolo
    q[sel_cpu]->push(pid);
    // Almaceno el pid del proceso entre los asociados al core
    tasks[sel_cpu]->push_back(pid);
    // Incremento el contador de proceso activos del core
    proc_count[sel_cpu]++;
}

void SchedRR2::unblock(int pid) {
    int cpu = find_cpu(pid);
    q[cpu]->push(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
    int curr_pid = current_pid(cpu);
    int next_pid = IDLE_TASK;

    switch (m) {
        // EL proceso estaba usando el cpu
        case TICK:
            if (curr_pid == IDLE_TASK) {
                // Si el cpu estaba al cuete
                next_pid = next(cpu);
            } else {
                // Si estaba corriendo algo
                if (cpu_quantum[cpu] > 0) {
                    // Si queda quantum, decremento el tiempo restante
                    cpu_quantum[cpu]--;
                    next_pid = curr_pid;
                } else {
                    // Si no queda quantum, desalojo y busco al siguiente proceso
                    q[cpu]->push(curr_pid);  // El actual vuelve a la cola
                    next_pid = next(cpu);
                }

            }
            break;

        // El proceso realizó una llamada bloqueante / estuvo bloqueado
        case BLOCK:
            next_pid = next(cpu);
            break;

        // El proceso terminó
        case EXIT:
            next_pid = next(cpu);

            // Elimino el proceso actual de la lista de tareas del cpu
            std::vector<int>::iterator i = find(tasks[cpu]->begin(), tasks[cpu]->end(), curr_pid);
            tasks[cpu]->erase(i);

            proc_count[cpu]--;  // Decremento el contador de tareas del cpu
            break;
    }

    return next_pid;
}

int SchedRR2::next(int cpu) {
    // Restauro el timer del quantum
    cpu_quantum[cpu] = def_quantum[cpu];

    // Devuelvo el pid del proceso siguiente y desencolo si es necesario
    int next_pid = -1;
    if (! q[cpu]->empty()) {
        next_pid = q[cpu]->front();
        q[cpu]->pop();
    }

    return next_pid;
}

int SchedRR2::find_cpu(int pid) {
    for (int i = 0; i < ncores; i++) {
        std::vector<int>::iterator j = find(tasks[i]->begin(), tasks[i]->end(), pid);
        if (j != tasks[i]->end()) {
            return i;
        }
    }
    return -1;
}

/*
 * Funciones auxiliares
 * -------------------- */

// Devuelve el índice del elemento mínimo de un std::vector<int>
unsigned int min_index(const std::vector<int> v) {
    int min = -1;

    if (v.size() > 0) {
        min = 0;
        for (unsigned int i = 1; i < v.size(); i++) {
            if (v[i] < v[min]) {
                min = i;
            }
        }
    }

    return min;
}
