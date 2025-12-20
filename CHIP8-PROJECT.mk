##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=CHIP8-PROJECT
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE
ProjectPath            :=C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/CHIP8-PROJECT
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=ARYAN
Date                   :=20/12/2025
CodeLitePath           :=D:/SOFTWARES/CodeLite
MakeDirCommand         :=mkdir
LinkerName             :=C:/msys64/clang64/bin/clang++.exe
SharedObjectLinkerName :=C:/msys64/clang64/bin/clang++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/clang64/bin/windres.exe
LinkOptions            :=  -lmingw32 -lSDL2main -lSDL2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)$(ProjectPath)/include $(IncludeSwitch)C:\msys64\clang64\include\SDL2 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)provided $(LibrarySwitch)provided $(LibrarySwitch)SDL2 $(LibrarySwitch)SDL2main $(LibrarySwitch)mingw32 
ArLibs                 :=  "provided.a" "provided" "SDL2" "SDL2main" "mingw32" 
LibPath                := $(LibraryPathSwitch)lib\mingw64 $(LibraryPathSwitch)C:\Users\ARYAN\CodeLiteProjects\CHIP8-WORKSPACE\CHIP8-PROJECT\lib\mingw64 $(LibraryPathSwitch)C:\msys64\clang64\lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/msys64/clang64/bin/ar.exe -r
CXX      := C:/msys64/clang64/bin/clang++.exe
CC       := C:/msys64/clang64/bin/clang.exe
CXXFLAGS :=  -gdwarf-2 -O0 -Wall $(Preprocessors)
CFLAGS   :=  -gdwarf-2 -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/clang64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=D:\SOFTWARES\CodeLite
Objects0=$(IntermediateDirectory)/src_main.c$(ObjectSuffix) $(IntermediateDirectory)/collection_ram_ram.c$(ObjectSuffix) $(IntermediateDirectory)/collection_exemple_exemple.c$(ObjectSuffix) $(IntermediateDirectory)/collection_rom_rom.c$(ObjectSuffix) $(IntermediateDirectory)/collection_processeur_processseur.c$(ObjectSuffix) $(IntermediateDirectory)/src_testrom.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_main.c$(ObjectSuffix): src/main.c 
	$(CC) $(SourceSwitch) "C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT/src/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.c$(PreprocessSuffix): src/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.c$(PreprocessSuffix) src/main.c

$(IntermediateDirectory)/collection_ram_ram.c$(ObjectSuffix): collection/ram/ram.c 
	$(CC) $(SourceSwitch) "C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT/collection/ram/ram.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/collection_ram_ram.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/collection_ram_ram.c$(PreprocessSuffix): collection/ram/ram.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/collection_ram_ram.c$(PreprocessSuffix) collection/ram/ram.c

$(IntermediateDirectory)/collection_exemple_exemple.c$(ObjectSuffix): collection/exemple/exemple.c 
	$(CC) $(SourceSwitch) "C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT/collection/exemple/exemple.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/collection_exemple_exemple.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/collection_exemple_exemple.c$(PreprocessSuffix): collection/exemple/exemple.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/collection_exemple_exemple.c$(PreprocessSuffix) collection/exemple/exemple.c

$(IntermediateDirectory)/collection_rom_rom.c$(ObjectSuffix): collection/rom/rom.c 
	$(CC) $(SourceSwitch) "C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT/collection/rom/rom.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/collection_rom_rom.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/collection_rom_rom.c$(PreprocessSuffix): collection/rom/rom.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/collection_rom_rom.c$(PreprocessSuffix) collection/rom/rom.c

$(IntermediateDirectory)/collection_processeur_processseur.c$(ObjectSuffix): collection/processeur/processseur.c 
	$(CC) $(SourceSwitch) "C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT/collection/processeur/processseur.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/collection_processeur_processseur.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/collection_processeur_processseur.c$(PreprocessSuffix): collection/processeur/processseur.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/collection_processeur_processseur.c$(PreprocessSuffix) collection/processeur/processseur.c

$(IntermediateDirectory)/src_testrom.c$(ObjectSuffix): src/testrom.c 
	$(CC) $(SourceSwitch) "C:/Users/ARYAN/CodeLiteProjects/CHIP8-WORKSPACE/CHIP8-PROJECT/src/testrom.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_testrom.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_testrom.c$(PreprocessSuffix): src/testrom.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_testrom.c$(PreprocessSuffix) src/testrom.c

##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


