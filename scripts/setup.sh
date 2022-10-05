#! /usr/bin/env bash
cd ../
vendor/bin/premake5 gmake2
vendor/bin/premake5 vscode
rm -rf Crane/vendor/GLAD/.vscode
rm -rf Crane/vendor/GLFW/.vscode
rm -rf Crane/vendor/imgui/.vscode
