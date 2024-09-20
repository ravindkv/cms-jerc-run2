
ROOT_I=-I`root-config --incdir` -I./header
ROOT_L=`root-config --libs`
CORR_A=`correction config --cflags --ldflags --rpath`
GCC=g++ -std=c++17
#GCC=g++ -g -std=c++17

SRCDIR   = cpp
OBJDIR   = obj
SOURCES     := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS     := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
BINS        := runMain

$(BINS): $(OBJECTS) main.cpp
	@echo "--> Creating executables $@"
	@$(GCC) -o $@ main.cpp ${CORR_A} ${ROOT_L} ${ROOT_I} $(OBJECTS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp header/%.h
	@echo "--> Creating object $@"
	@mkdir -p $(OBJDIR)
	@$(GCC) -c $< -o $@ ${CORR_A} ${ROOT_L} ${ROOT_I}

clean:
	rm -f $(wildcard $(OBJDIR)/*.o) $(BINS)

.PHONY: all clean

