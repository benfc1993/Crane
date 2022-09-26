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

	p.w('],\n\"tasks\": {\n\t\"version\": \"2.0.0\",\n\t\"tasks\": [\n\t\t{\n\t\t\t\"type\": \"shell\",\n\t\t\t\"label\": \"Premake\",\n\t\t\t\"command\": \"cd ${workspaceFolder}/../scripts && ./setup.sh\",\n\t\t\t\"args\": [],\n\t\t\t\"group\": \"none\",\n\t\t\t\"detail\": \"Premake: Create build files\",\n\t\t\t\"problemMatcher\": []\n\t\t},\n\t\t{\n\t\t\t\"type\": \"cppbuild\",\n\t\t\t\"label\": \"build debug\",\n\t\t\t\"command\": \"/usr/bin/make\",\n\t\t\t\"args\": [],\n\t\t\t\"options\": {\n\t\t\t\t\"cwd\": \"${workspaceFolder}/..\"\n\t\t\t},\n\t\t\t\"problemMatcher\": [\n\t\t\t\t\"$gcc\"\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in debug config\"\n\t\t},\n\t\t{\n\t\t\t\"type\": \"cppbuild\",\n\t\t\t\"label\": \"build release\",\n\t\t\t\"command\": \"/usr/bin/make\",\n\t\t\t\"args\": [\n\t\t\t\t\"config=release\"\n\t\t\t],\n\t\t\t\"options\": {\n\t\t\t\t\"cwd\": \"${workspaceFolder}/..\"\n\t\t\t},\n\t\t\t\"problemMatcher\": [\n\t\t\t\t\"$gcc\"\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in release config\"\n\t\t},\n\t\t{\n\t\t\t\"type\": \"cppbuild\",\n\t\t\t\"label\": \"build dist\",\n\t\t\t\"command\": \"/usr/bin/make\",\n\t\t\t\"args\": [\n\t\t\t\t\"config=dist\",\n\t\t\t],\n\t\t\t\"options\": {\n\t\t\t\t\"cwd\": \"${workspaceFolder}/..\"\n\t\t\t},\n\t\t\t\"problemMatcher\": [\n\t\t\t\t\"$gcc\"\n\t\t\t],\n\t\t\t\"group\": \"build\",\n\t\t\t\"detail\": \"compiler: make in distribution config\"\n\t\t}\n\t]\n},\n\"settings\": {\n\t\"editor.defaultFormatter\": \"ms-vscode.cpptools\",\n\t\"editor.formatOnSave\": true,\n\t\"C_Cpp.formatting\": \"vcFormat\",\n\t\"C_Cpp.vcFormat.newLine.beforeOpenBrace.type\": \"newLine\",\n\t\"C_Cpp.vcFormat.newLine.beforeOpenBrace.block\": \"newLine\",\n\t\"C_Cpp.vcFormat.newLine.beforeOpenBrace.function\": \"newLine\",\n\t\"githubPullRequests.fileListLayout\": \"tree\"\n}\n}')

	--TODO wks.startproject
end
