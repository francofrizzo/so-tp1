#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
//<<<REMOVE>>>
		std::vector< std::queue<int> *> q;
		std::vector< std::vector<int> *> tasks;
		std::vector<int> cpu_quantum;
		std::vector<int> def_quantum;
		int	lastcpu;
		int ncores;
//<<<REMOVE END>>>
		int next(int cpu);
};

#endif
