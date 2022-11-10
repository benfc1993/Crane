# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

ifeq ($(config),debug)
  CSC = csc
  RESGEN = resgen
TARGETDIR = Bin
TARGET = $(TARGETDIR)/SandboxProject.dll
OBJDIR = Intermediates/Debug
  FLAGS = /noconfig
  DEPENDS = ../Scripts/Crane-ScriptCore.dll
  REFERENCES = /r:../Scripts/Crane-ScriptCore.dll
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef
endif

ifeq ($(config),release)
  CSC = csc
  RESGEN = resgen
TARGETDIR = Bin
TARGET = $(TARGETDIR)/SandboxProject.dll
OBJDIR = Intermediates/Release
  FLAGS = /optimize /noconfig
  DEPENDS = ../Scripts/Crane-ScriptCore.dll
  REFERENCES = /r:../Scripts/Crane-ScriptCore.dll
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef
endif

ifeq ($(config),dist)
  CSC = csc
  RESGEN = resgen
TARGETDIR = Bin
TARGET = $(TARGETDIR)/SandboxProject.dll
OBJDIR = Intermediates/Dist
  FLAGS = /noconfig
  DEPENDS = ../Scripts/Crane-ScriptCore.dll
  REFERENCES = /r:../Scripts/Crane-ScriptCore.dll
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef
endif

FLAGS += /t:library 
REFERENCES += 

SOURCES += \
	Assets/Scripts/Source/CameraController.cs \
	Assets/Scripts/Source/Player.cs \

EMBEDFILES += \

RESPONSE += $(OBJDIR)/SandboxProject.rsp
SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

all: prebuild $(EMBEDFILES) $(COPYFILES) $(TARGET)

$(TARGET): $(SOURCES) $(EMBEDFILES) $(DEPENDS) $(RESPONSE) | $(TARGETDIR)
	$(PRELINKCMDS)
	$(SILENT) $(CSC) /nologo /out:$@ $(FLAGS) $(REFERENCES) @$(RESPONSE) $(patsubst %,/resource:%,$(EMBEDFILES))
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(RESPONSE): SandboxProject.make
	@echo Generating response file
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f $(RESPONSE)
else
	$(SILENT) if exist $(RESPONSE) del $(OBJDIR)\SandboxProject.rsp
endif
	@echo Assets/Scripts/Source/CameraController.cs >> $(RESPONSE)
	@echo Assets/Scripts/Source/Player.cs >> $(RESPONSE)

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)
