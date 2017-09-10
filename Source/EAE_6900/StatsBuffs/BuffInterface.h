#pragma once

// engine includes

// game includes
#include "Buff.h"

#include "BuffInterface.generated.h"

UINTERFACE()
class UBuffInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class EAE_6900_API IBuffInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual const TArray<FBuffEffect>& GetCurrentlyActiveBuffs() const = 0;

	virtual void AddBuff(const FBuffEffect& NewBuff) = 0;

};
