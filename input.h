#ifndef _INPUT_H_
#define _INPUT_H_

struct phys_object;

extern int input_init(void);

extern void input_scan_joysticks(void);

extern void handle_events(void);

#endif
