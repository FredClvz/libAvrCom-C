/*
 * ringbuffer_test.c
 *
 * Test cases for the ringbuffer module.
 */

#include "CuTest.h"

#include "ringbuffer.h"

#define NVAL  10

RNGB_Buffer tb;
bdt data[NVAL] = {0};

void TestBufferInit(CuTest* tc)
{
	RNGB_Init(&tb, data, NVAL);

	CuAssertIntEquals(tc, 0, tb.IdxIn);
	CuAssertIntEquals(tc, tb.IdxOut, tb.u8Size);
	CuAssertIntEquals(tc, NVAL, tb.u8Size);
	CuAssertPtrNotNull(tc, tb.pBuffer);
	CuAssertPtrEquals(tc, data, tb.pBuffer);
}

