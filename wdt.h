#ifndef WDT_H_
#define WDT_H_

#include <msp430.h>

inline void wdt_off()
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
}

#endif /* WDT_H_ */
