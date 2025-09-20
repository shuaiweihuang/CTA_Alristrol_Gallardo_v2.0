#pragma once

#ifndef WAIT_TIMEOUT
#include <errno.h>
#define WAIT_TIMEOUT ETIMEDOUT
#endif

#include <stdint.h>

namespace neosmart
{
	//Type declarations
	struct neosmart_event_t_;
	typedef neosmart_event_t_ * neosmart_event_t;

	//WIN32-style functions
	neosmart_event_t CreateEvent(bool manualReset, bool initialState);
	int DestroyEvent(neosmart_event_t event);
	int WaitForEvent(neosmart_event_t event, uint64_t milliseconds);
	int SetEvent(neosmart_event_t event);
	int ResetEvent(neosmart_event_t event);
	int WaitForMultipleEvents(neosmart_event_t *events, int count, bool waitAll, uint64_t milliseconds);
	int WaitForMultipleEvents(neosmart_event_t *events, int count, bool waitAll, uint64_t milliseconds, int &index);
}
