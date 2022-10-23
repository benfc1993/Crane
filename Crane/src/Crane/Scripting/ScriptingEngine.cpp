#include "crpch.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
// #include "mono/metadata/tabledefs.h"

void Test()
{
    mono_set_assemblies_path("mono/lib");
}