#source file search paths
VPATH = ./src ../PathFinder/src

# compiler include search paths
INCS = -I../raven-set \
	-I../windex/include \
	-I../PathFinder/src \


LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \

	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static -lComctl32

# folders
ODIR = ./.vscode/obj

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_starter = \
	$(ODIR)/main.o \
	$(ODIR)/shunter.o \
	$(ODIR)/cGUI.o \
	$(ODIR)/generate.o \
	$(ODIR)/cGraph.o \
	$(ODIR)/GraphTheory.o \

### COMPILE

$(ODIR)/%.o: %.cpp  
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

starter: $(OBJ_starter)
	g++ -o bin/shunter.exe \
	$^ $(OPT) \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

