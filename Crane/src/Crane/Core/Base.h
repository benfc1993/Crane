#pragma once

#include <sstream>
#include <memory>

#define BIT(x) (1 << x)
#define CR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include "Crane/Core/Log.h"
#include "Crane/Core/Assert.h"
#include "Crane/Core/Time.h"

namespace Crane {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

}