#! /usr/bin/env bash
dotnet build $(dirname "$0")/Source/CraneScriptCore.csproj -o ../Sparrow/Resources/Scripts
dotnet pack $(dirname "$0")/Source/CraneScriptCore.csproj
rm -rf $(dirname "$0")/Source/obj
rm -rf $(dirname "$0")/Source/bin