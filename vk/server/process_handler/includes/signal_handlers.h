#ifndef __SIGNAL_HANDLERS_H__
#define __SIGNAL_HANDLERS_H__

void configSignals();
void threadSidHandler();
void sigHandler(int nsig);

#endif /* __SIGNAL_HANDLERS_H__ */
