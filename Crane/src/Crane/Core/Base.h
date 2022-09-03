#include <sstream>

#define BIT(x) (1 << x)
#define CR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include "Crane/Core/Log.h"
#include "Crane/Core/Assert.h"
