--
-- Name:        vscode_workspace.lua
-- Purpose:     Generate a vscode file.
-- Author:      Ryan Pusztai
-- Modified by: Andrea Zanellato
--              Manu Evans
--              Yehonatan Ballas
-- Created:     2013/05/06
-- Copyright:   (c) 2008-2020 Jason Perkins and the Premake project
--

local p = premake
local project = p.project
local workspace = p.workspace
local tree = p.tree
local vscode = p.modules.vscode

vscode.workspace = {}
local m = vscode.workspace

--
-- Generate a vscode file
--
function m.generate(wks)
	p.utf8()
	p.w('{"folders": [')

	--
	-- Project list
	--
	local tr = workspace.grouptree(wks)
	tree.traverse(tr, {
		onleaf = function(n)
			local prj = n.project

			-- Build a relative path from the workspace file to the project file
            local prjpath = path.getrelative(prj.workspace.location, prj.location)
				p.w('{')
				p.w('"path": "%s"', prjpath)
				p.w('},')
		end,
	})

	p.w('{\"path\":\"Crane-ScriptCore\"}],\n\"tasks\": {\n\t\"version\": \"2.0.0\",\n\t\"tasks\": [\n\t\t{\n\t\t\t\"type\": \"shell\",\n\t\t\t\"label\": \"Premake\",\n\t\t\t\"command\": \"cd ${workspaceFolder}/../scripts && ./setup.sh\",\n\t\t\t\"args\": [],\n\t\t\t\"group\": \"none\",\n\t\t\t\"detail\": \"Premake: Create build files\",\n\t\t\t\"problemMatcher\": []\n\t\t},\n\t\t{\n\t\t\t\"type\": \"cppbuild\",\n\t\t\t\"label\": \"build debug\",\n\t\t\t\"command\": \"/usr/bin/make\",\n\t\t\t\"args\": [],\n\t\t\t\"options\": {\n\t\t\t\t\"cwd\": \"${workspaceFolder}/..\"\n\t\t\t},\n\t\t\t\"problemMatcher\": [\n\t\t\t\t\"$gcc\"\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in debug config\"\n\t\t},\n\t\t{\n\t\t\t\"type\": \"cppbuild\",\n\t\t\t\"label\": \"build release\",\n\t\t\t\"command\": \"/usr/bin/make\",\n\t\t\t\"args\": [\n\t\t\t\t\"config=release\"\n\t\t\t],\n\t\t\t\"options\": {\n\t\t\t\t\"cwd\": \"${workspaceFolder}/..\"\n\t\t\t},\n\t\t\t\"problemMatcher\": [\n\t\t\t\t\"$gcc\"\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in release config\"\n\t\t},\n\t\t{\n\t\t\t\"type\": \"cppbuild\",\n\t\t\t\"label\": \"build dist\",\n\t\t\t\"command\": \"/usr/bin/make\",\n\t\t\t\"args\": [\n\t\t\t\t\"config=dist\",\n\t\t\t],\n\t\t\t\"options\": {\n\t\t\t\t\"cwd\": \"${workspaceFolder}/..\"\n\t\t\t},\n\t\t\t\"problemMatcher\": [\n\t\t\t\t\"$gcc\"\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in distribution config\"\n\t\t},\n\t\t{\n\t\t\t\"type\": \"shell\",\n\t\t\t\"label\": \"build script core\",\n\t\t\t\"command\": \"/usr/bin/bash\",\n\t\t\t\"args\": [\n\t\t\t\t\"${workspaceFolder:Crane-ScriptCore}/build.sh\",\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in distribution config\"\n\t\t}\n\t]\n},\n\"launch\": {\"version\": \"0.2.0\",\n  \"configurations\": [\n    {\n      \"name\": \"Debug - Sandbox\",\n      \"type\": \"cppdbg\",\n      \"request\": \"launch\",\n      \"program\": \"${workspaceFolder:Sandbox}/../bin/Debug-linux-x86_64/Sandbox/Sandbox\", \n      \"args\": [],\n      \"stopAtEntry\": false,\n      \"cwd\": \"${workspaceFolder:Sandbox}\",\n      \"environment\": [{\"name\": \"config\", \"value\": \"Debug\"}],\n      \"externalConsole\": false,\n      \"MIMode\": \"gdb\",\n      \"setupCommands\": [\n        {\n          \"description\": \"Enable pretty-printing for gdb\",\n          \"text\": \"-enable-pretty-printing\",\n          \"ignoreFailures\": true\n        },\n        {\n          \"description\": \"Set Disassembly Flavor to Intel\",\n          \"text\": \"-gdb-set disassembly-flavor intel\",\n          \"ignoreFailures\": true\n        }\n      ],\n      \"preLaunchTask\": \"build debug\"\n    },\n    {\n      \"name\": \"Release - Sandbox\",\n      \"type\": \"cppdbg\",\n      \"request\": \"launch\",\n      \"program\": \"${workspaceFolder:Sandbox}/../bin/Release-linux-x86_64/Sandbox/Sandbox\", \n      \"args\": [],\n      \"stopAtEntry\": false,\n      \"cwd\": \"${workspaceFolder:Sandbox}\",\n      \"environment\": [{\"name\": \"config\", \"value\": \"Release\"}],\n      \"externalConsole\": false,\n      \"MIMode\": \"gdb\",\n      \"setupCommands\": [\n        {\n          \"description\": \"Enable pretty-printing for gdb\",\n          \"text\": \"-enable-pretty-printing\",\n          \"ignoreFailures\": true\n        },\n        {\n          \"description\": \"Set Disassembly Flavor to Intel\",\n          \"text\": \"-gdb-set disassembly-flavor intel\",\n          \"ignoreFailures\": true\n        }\n      ],\n      \"preLaunchTask\": \"build release\"\n    },\n    {\n      \"name\": \"Dist - Sandbox\",\n      \"type\": \"cppdbg\",\n      \"request\": \"launch\",\n      \"program\": \"${workspaceFolder:Sandbox}/../bin/Dist-linux-x86_64/Sandbox/Sandbox\", \n      \"args\": [],\n      \"stopAtEntry\": false,\n      \"cwd\": \"${workspaceFolder:Sandbox}\",\n      \"environment\": [{\"name\": \"config\", \"value\": \"Dist\"}],\n      \"externalConsole\": false,\n      \"MIMode\": \"gdb\",\n      \"setupCommands\": [\n        {\n          \"description\": \"Enable pretty-printing for gdb\",\n          \"text\": \"-enable-pretty-printing\",\n          \"ignoreFailures\": true\n        },\n        {\n          \"description\": \"Set Disassembly Flavor to Intel\",\n          \"text\": \"-gdb-set disassembly-flavor intel\",\n          \"ignoreFailures\": true\n        }\n      ],\n      \"preLaunchTask\": \"build dist\"\n    },\n\n        {\n      \"name\": \"Debug - Sparrow\",\n      \"type\": \"cppdbg\",\n      \"request\": \"launch\",\n      \"program\": \"${workspaceFolder:Sparrow}/../bin/Debug-linux-x86_64/Sparrow/Sparrow\", \n      \"args\": [],\n      \"stopAtEntry\": false,\n      \"cwd\": \"${workspaceFolder:Sparrow}\",\n      \"environment\": [{\"name\": \"config\", \"value\": \"Debug\"}],\n      \"externalConsole\": false,\n      \"MIMode\": \"gdb\",\n      \"setupCommands\": [\n        {\n          \"description\": \"Enable pretty-printing for gdb\",\n          \"text\": \"-enable-pretty-printing\",\n          \"ignoreFailures\": true\n        },\n        {\n          \"description\": \"Set Disassembly Flavor to Intel\",\n          \"text\": \"-gdb-set disassembly-flavor intel\",\n          \"ignoreFailures\": true\n        }\n      ],\n      \"preLaunchTask\": \"build debug\"\n    },\n    {\n      \"name\": \"Release - Sparrow\",\n      \"type\": \"cppdbg\",\n      \"request\": \"launch\",\n      \"program\": \"${workspaceFolder:Sparrow}/../bin/Release-linux-x86_64/Sparrow/Sparrow\", \n      \"args\": [],\n      \"stopAtEntry\": false,\n      \"cwd\": \"${workspaceFolder:Sparrow}\",\n      \"environment\": [{\"name\": \"config\", \"value\": \"Release\"}],\n      \"externalConsole\": false,\n      \"MIMode\": \"gdb\",\n      \"setupCommands\": [\n        {\n          \"description\": \"Enable pretty-printing for gdb\",\n          \"text\": \"-enable-pretty-printing\",\n          \"ignoreFailures\": true\n        },\n        {\n          \"description\": \"Set Disassembly Flavor to Intel\",\n          \"text\": \"-gdb-set disassembly-flavor intel\",\n          \"ignoreFailures\": true\n        }\n      ],\n      \"preLaunchTask\": \"build release\"\n    },\n    {\n      \"name\": \"Dist - Sparrow\",\n      \"type\": \"cppdbg\",\n      \"request\": \"launch\",\n      \"program\": \"${workspaceFolder:Sparrow}/../bin/Dist-linux-x86_64/Sparrow/Sparrow\", \n      \"args\": [],\n      \"stopAtEntry\": false,\n      \"cwd\": \"${workspaceFolder:Sparrow}\",\n      \"environment\": [{\"name\": \"config\", \"value\": \"Dist\"}],\n      \"externalConsole\": false,\n      \"MIMode\": \"gdb\",\n      \"setupCommands\": [\n        {\n          \"description\": \"Enable pretty-printing for gdb\",\n          \"text\": \"-enable-pretty-printing\",\n          \"ignoreFailures\": true\n        },\n        {\n          \"description\": \"Set Disassembly Flavor to Intel\",\n          \"text\": \"-gdb-set disassembly-flavor intel\",\n          \"ignoreFailures\": true\n        }\n      ],\n      \"preLaunchTask\": \"build dist\"\n    },\n  ]\n},\n\"settings\": {\n\t\"editor.defaultFormatter\": \"ms-vscode.cpptools\",\n\t\"editor.formatOnSave\": true,\n\t\"C_Cpp.formatting\": \"vcFormat\",\n\t\"C_Cpp.vcFormat.newLine.beforeOpenBrace.type\": \"newLine\",\n\t\"C_Cpp.vcFormat.newLine.beforeOpenBrace.block\": \"newLine\",\n\t\"C_Cpp.vcFormat.newLine.beforeOpenBrace.function\": \"newLine\",\n\t\"C_Cpp.vcFormat.indent.lambdaBracesWhenParameter\": false,\n\t\"githubPullRequests.fileListLayout\": \"tree\",\n\t\"C_Cpp.default.browse.limitSymbolsToIncludedHeaders\": true,\n\t\"C_Cpp.intelliSenseCacheSize\": 0\n}\n}')

	--TODO wks.startproject
end
