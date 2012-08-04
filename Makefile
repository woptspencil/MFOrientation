# Compiler 
CXX = g++
# Cflags 
CFLAGS = -Wall -ansi -g -O3 
# Source Folder
SDIR = src
# Object Folder
ODIR = obj
# Objects
_OBJS = Data.o MFSolver.o

# Objects in folder
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# Output
MF = MFOrientation

all: $(MF)

$(MF): $(OBJS) 
	$(CXX) -o $@ $(OBJS) $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(SDIR)/%.h
	-@mkdir -p $(ODIR)
	$(CXX) -c -o $@ $< $(CFLAGS) 

clean: 
	rm -f $(ODIR)/*.o $(OUT)
	rm -rf *~
