#pragma once

namespace rt
{

class ICloneable 
{
public:
	virtual ~ICloneable() {}
	virtual ICloneable* Clone() const = 0;
}; // ICloneable

}
