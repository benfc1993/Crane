#! /usr/bin/env bash
echo "Creating directory"
echo "----||----"
cd $1
mkdir -p $2/Assets
cd $2

cp $3/Directory.Build.props Directory.Build.props;

echo "Creating solution and project"
echo "----||----"
dotnet new sln
dotnet new classlib --name $2 --output Assets/Scripts --language "C#" --framework netstandard2.0; 
dotnet sln add --in-root Assets/Scripts

# add csproj file
cd Assets/Scripts
rm -rf $2.csproj
cp $3/project.xml $2.csproj;

echo "Adding default Assets"
echo "----||----"
# Create scenes dir and default scene file
cd ../
mkdir Scenes
cd Scenes
touch default.scene
echo "Scene:" >> default.scene
echo "  Name: default" >> default.scene
echo "  FilePath: assets/scenes/default.scene" >> default.scene
echo "Entities:" >> default.scene
cd ../../

echo "Linking Crane Script Core"
echo "----||----"
# Add Script core package
touch nuget.config
echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> nuget.config
echo "<configuration>" >> nuget.config
echo "    <packageSources>" >> nuget.config
echo "<add key=\"crane-pcakges\" value=\"/home/ben/projects/crane/Crane-ScriptCore/Source/bin/Debug\" />" >> nuget.config
echo "    </packageSources>" >> nuget.config
echo "</configuration>" >> nuget.config
dotnet nuget locals all --clear
dotnet restore Assets/Scripts
cd Assets/Scripts
dotnet add package CraneScriptCore

# rm -rf obj

echo "Creating Crane Project file"
echo "----||----"
# add crproj file
cd ../../
touch $2.crproj
echo "Project:" >> $2.crproj
echo "  Name: NewProject" >> $2.crproj
echo "  ProjectRootDir: /home/ben/projects/NewProject" >> $2.crproj
echo "  AssetDir: Assets" >> $2.crproj
echo "  AssemblyPath: bin/NewProject.dll" >> $2.crproj
echo "  StartScene: default.scene" >> $2.crproj

# add vscode tasks
cp -r $3/.vscode .vscode

echo "Building assembly"
echo "----||----"
# build
dotnet build $2.sln -o bin
