#ifndef _AFL_CALL_H
#define _AFL_CALL_H

#ifdef __cplusplus
extern "C" {
#endif
/* afl_call.c/.cpp */
int startForkserver(int ticks);
int doneWork(int val);
#ifdef __cplusplus
}
#endif

#endif
