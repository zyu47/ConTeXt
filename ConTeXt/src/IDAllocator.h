
#ifndef _IDALLOCATOR_H
#define _IDALLOCATOR_H
#include "StdAfx.h"
class IDAllocator
{
public:
	virtual bool allocate(size_t quantity, idx_t *start) = 0;
};

#endif

