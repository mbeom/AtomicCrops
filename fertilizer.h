#pragma once
#include "Item.h"

class fertilizer : public Item
{
private:

public:

	virtual HRESULT init();
	virtual HRESULT init(float x, float y, bool is_taken);
	virtual void release();
};

