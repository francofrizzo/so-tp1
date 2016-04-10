#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
//<<<REMOVE>>>
		std::queue<int> q;
		std::vector<int> cpu_quantum;
		std::vector<int> def_quantum;
		int next(int cpu);
//<<<REMOVE END>>>
};

#endif
