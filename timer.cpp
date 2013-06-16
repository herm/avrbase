#include "timer.h"

#include <avr/interrupt.h>

class SystemTimer
{
public:
    static void init();
    #ifdef TIMER16
    static force_inline timer_t read_irq_disabled() { return TCNT1; }
    static force_inline void set_next_trigger_irq_disabled(timer_t value) { OCR1A = value; TIMSK |= _BV(OCF1A);}
    #endif
    static void add_event_irq_disabled(TimerEvent *event);
};

static TimerEvent *timer_head=0;

/* This code is inspired by the MBED sources. */
ISR(TIMER1_COMPA_vect)
{
    while (1)
    {
        if (!timer_head)
        {
            //Disable capture IRQ
            TIMSK &= ~_BV(OCF1A);
            return;
        }
        if ((stimer_t)(timer_head->timestamp - SystemTimer::read_irq_disabled()) <= 0)
        {
            TimerEvent *p = timer_head;
            timer_head = p->next;
            if (p->callback) p->callback();
            if (p->interval)
            {
                //Queue this event again
                p->timestamp += p->interval;
                SystemTimer::add_event_irq_disabled(p);
            }
        } else {
            //next event is in the future
            SystemTimer::set_next_trigger_irq_disabled(timer_head->timestamp);
            return;
        }
    }
}



void SystemTimer::init()
{
    TCCR1A = 0;
    TCCR1B = _BV(CS12) | _BV(CS10); //Divide by 1024
}

//Code borrowed from MBED
void SystemTimer::add_event_irq_disabled(TimerEvent *event)
{
    SystemTimer::init(); //TODO
    TimerEvent *prev = 0, *p = timer_head;
    while (p)
    {
        /* check if we come before p */
        if ((stimer_t)(event->timestamp - p->timestamp) <= 0) {
            break;
        }
        /* go to the next element */
        prev = p;
        p = p->next;
    }
    /* if prev is NULL we're at the head */
    if (!prev) {
        timer_head = event;
        SystemTimer::set_next_trigger_irq_disabled(event->timestamp);
    } else {
        prev->next = event;
    }
    /* if we're at the end p will be NULL, which is correct */
    event->next = p;
}


void TimerEvent::attach_ticks(irq_func_t func, timer_t ticks)
{
    callback = func;
    SystemTimer::init(); //TODO
    uint8_t sreg = SREG;
    timestamp = SystemTimer::read_irq_disabled() + ticks;
    SystemTimer::add_event_irq_disabled(this);
    SREG = sreg;
}
