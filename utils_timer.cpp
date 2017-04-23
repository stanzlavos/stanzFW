#include "utils_timer.h"
#include <RingBufCPP.h>

extern "C" {
#include "user_interface.h"
}

#define MAX_OS_TIMERS           6
#define MAX_TIMER_EVENTS        16
#define MAX_TIMER_INTERVAL      60  // in sec

os_timer_t _timers[MAX_OS_TIMERS];

typedef struct
{
    os_timer_t      *timer;
    bool            repeat;
    uint16_t		repeat_cnt;
    timer_func_t    func;
    void            *arg;
}timer_node_t;

RingBufCPP<timer_node_t *, MAX_OS_TIMERS>       TimerRing;
RingBufCPP<timer_node_t *, MAX_TIMER_EVENTS>    TimerEventRing;

void init_timer_ring(void)
{
    uint8_t i = 0;
    for( ; i < MAX_OS_TIMERS; i ++)
    {
        timer_node_t *timerNode = new timer_node_t;
        timerNode->timer      	= &_timers[i];
        timerNode->repeat     	= false;
        timerNode->repeat_cnt 	= 0;
        timerNode->func       	= NULL;
        timerNode->arg        	= NULL;

        TimerRing.add(timerNode);
    }
}

void timer_handler(void *arg)
{
    timer_node_t *timerNode = (timer_node_t *)arg;
    TimerEventRing.add(timerNode);    
}

void free_timer(timer_node_t *node)
{
	bool free_timer = false;

	if(!(node->repeat))
	{
		free_timer = true;
	}
	else
	{
		if(node->repeat_cnt && (--node->repeat_cnt == 0))
		{
			os_timer_disarm(node->timer);
			free_timer = true;
		}
	}

	if(free_timer)
	{
		node->repeat     	= false;
		node->repeat_cnt 	= 0;
		node->func       	= NULL;
		node->arg        	= NULL;

		TimerRing.add(node);
	}
}

void handle_timer_events(void)
{
    while(!TimerEventRing.isEmpty())
    {
        timer_node_t *timerNode = NULL;
        TimerEventRing.pull(&timerNode);

        timerNode->func(timerNode->arg);
        
        free_timer(timerNode);
    }
}

bool add_timer(timer_func_t func, void *arg, uint8_t intrvl, bool repeat, uint16_t repeat_cnt)
{
    if((!TimerRing.isEmpty()) && (func) && (intrvl <= MAX_TIMER_INTERVAL))
    {
        timer_node_t    *timerNode  = NULL;
        TimerRing.pull(&timerNode);
        os_timer_t      *timer      = timerNode->timer;
        timerNode->repeat           = repeat;
        timerNode->repeat_cnt       = repeat_cnt;
        timerNode->func             = func;
        timerNode->arg              = arg;

        os_timer_disarm(timer);
        os_timer_setfn(timer, timer_handler, (void *)timerNode);
        os_timer_arm(timer, (intrvl*1000), repeat);

        return true;
    }

    return false;
}

