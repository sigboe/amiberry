/*
* UAE - The Un*x Amiga Emulator
*
* Event stuff
*
* Copyright 1995-2002 Bernd Schmidt
* Copyright 1995 Alessandro Bissacco
* Copyright 2000-2012 Toni Wilen
*/
#include "sysdeps.h"

#include "options.h"
#include "include/memory.h"
#include "newcpu.h"

unsigned long int nextevent, currcycle;
int is_syncline;

frame_time_t vsyncmintime, vsyncmaxtime, vsyncwaittime;
int vsynctimebase;

void events_schedule(void)
{
	unsigned long int mintime = ~0L;
	for (auto& i : eventtab)
	{
		if (i.active)
		{
			auto eventtime = i.evtime - currcycle;
			if (eventtime < mintime)
				mintime = eventtime;
		}
	}
	nextevent = currcycle + mintime;
}

void do_cycles_cpu_fastest(unsigned long cycles_to_add)
{
	if ((regs.pissoff -= cycles_to_add) > 0)
		return;

	cycles_to_add = -regs.pissoff;
	regs.pissoff = 0;

	/* Keep only CPU emulation running while waiting for sync point. */
	if (is_syncline)
	{
		int rpt = read_processor_time();
		int v = rpt - vsyncmintime;
		if (v > vsynctimebase || v < -vsynctimebase)
			v = 0;
		if (v < speedup_timelimit)
		{
			regs.pissoff = pissoff_value;
			return;
		}
		is_syncline = 0;
	}

	while ((nextevent - currcycle) <= cycles_to_add)
	{
		cycles_to_add -= (nextevent - currcycle);
		currcycle = nextevent;

		for (auto& i : eventtab)
			if (i.active && i.evtime == currcycle)
				(*i.handler)();
		events_schedule();
	}
	currcycle += cycles_to_add;
}

void do_cycles_cpu_norm(unsigned long cycles_to_add)
{
	while ((nextevent - currcycle) <= cycles_to_add)
	{
		cycles_to_add -= (nextevent - currcycle);
		currcycle = nextevent;

		for (auto& i : eventtab)
			if (i.active && i.evtime == currcycle)
				(*i.handler)();
		events_schedule();
	}
	currcycle += cycles_to_add;
}

do_cycles_func do_cycles = do_cycles_cpu_norm;

void MISC_handler(void)
{
	static bool dorecheck;
	bool recheck;
	evt mintime = 0;
	evt ct = get_cycles();
	static int recursive;

	if (recursive)
	{
		dorecheck = true;
		return;
	}
	recursive++;
	eventtab[ev_misc].active = 0;
	recheck = true;
	while (recheck)
	{
		recheck = false;
		mintime = ~0L;
		for (auto& i : eventtab2)
		{
			if (i.active)
			{
				if (i.evtime == ct)
				{
					i.active = false;
					i.handler(i.data);
					if (dorecheck || i.active)
					{
						recheck = true;
						dorecheck = false;
					}
				}
				else
				{
					evt eventtime = i.evtime - ct;
					if (eventtime < mintime)
						mintime = eventtime;
				}
			}
		}
	}
	if (mintime != ~0UL)
	{
		eventtab[ev_misc].active = true;
		eventtab[ev_misc].evtime = ct + mintime;
		events_schedule();
	}
	recursive--;
}

void event2_newevent_xx(int no, evt t, uae_u32 data, evfunc2 func)
{
	static int next = ev2_misc;

	evt et = t + get_cycles();
	if (no < 0)
	{
		no = next;
		for (;;)
		{
			if (!eventtab2[no].active)
				break;
			if (eventtab2[no].evtime == et && eventtab2[no].handler == func && eventtab2[no].data == data)
				break;
			no++;
			if (no == ev2_max)
				no = ev2_misc;
			if (no == next)
			{
				write_log(_T("out of event2's!\n"));
				return;
			}
		}
		next = no;
	}
	eventtab2[no].active = true;
	eventtab2[no].evtime = et;
	eventtab2[no].handler = func;
	eventtab2[no].data = data;
	MISC_handler();
}

void event2_newevent_x_replace(evt t, uae_u32 data, evfunc2 func)
{
	for (auto& i : eventtab2)
	{
		if (i.active && i.handler == func)
			i.active = false;
	}
	if (int(t) <= 0)
	{
		func(data);
		return;
	}
	event2_newevent_xx(-1, t * CYCLE_UNIT, data, func);
}
