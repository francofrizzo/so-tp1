#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
  // MFQ recibe los quantums por parÃ¡metro
}

void SchedNoMistery::load(int pid) {
}

void SchedNoMistery::unblock(int pid) {
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
}

int SchedNoMistery::next(void) {
  // Elijo el nuevo pid
}
