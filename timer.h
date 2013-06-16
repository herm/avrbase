#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <stdint.h>
#include "config.h"
#include "utils.h"

#define TIMER_PRESCALER 1024
#define F_TIMER (F_CPU/TIMER_PRESCALER)

#ifdef TIMER16
    typedef uint16_t timer_t;
    typedef int16_t stimer_t;
#else
    typedef uint32_t timer_t;
    typedef int32_t stimer_t;
#warning "32bit timers don't work yet"
#endif

class TimerEvent;

class TimerEvent
{
public:
    TimerEvent *next;
    irq_func_t callback;
    timer_t timestamp;
    timer_t interval;
protected:
    void attach_ticks(irq_func_t func, timer_t ticks);
};


class Timeout : public TimerEvent
{
public:
    force_inline void attach(irq_func_t func, double time)
    {
        attach_ticks(func, (timer_t)(time*F_TIMER));
    }

    force_inline void attach_us(irq_func_t func, double time)
    {
        attach(func, time/1000.0);
    }

    force_inline void attach_ticks(irq_func_t func, timer_t ticks)
    {
        interval = 0;
        TimerEvent::attach_ticks(func, ticks);
    }

};

class Ticker : public TimerEvent
{
public:
    force_inline void attach(irq_func_t func, double time)
    {
        attach_ticks(func, (timer_t)(time*F_TIMER));
    }

    force_inline void attach_us(irq_func_t func, double time)
    {
        attach(func, time/1000.0);
    }

    force_inline void attach_ticks(irq_func_t func, timer_t ticks)
    {
        interval = ticks;
        TimerEvent::attach_ticks(func, ticks);
    }
};


#endif // TIMER_H
