#! /usr/bin/env bash
cd ../
vendor/bin/premake5 gmake2
vendor/bin/premake5 vscode
mv Crane/Crane/.vscode/c_cpp_properties.json .vscode
rm -rf Crane/Crane
rm -rf Sandbox/Sandbox
rm -rf Crane/vendor/GLAD/GLAD
rm -rf Crane/vendor/GLFW/GLFW
