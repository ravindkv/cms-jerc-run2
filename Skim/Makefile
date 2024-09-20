
ROOT_L=`root-config --libs`
ROOT_I=-I`root-config --incdir` -I./header
GCC=g++ -std=c++17

SRCDIR   = cpp
OBJDIR   = obj
SOURCES     := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS     := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
BINS        := runMain

$(BINS): $(OBJECTS) main.cpp
	@echo "--> Creating executables $@"
	@$(GCC) -o $@ main.cpp ${ROOT_I} ${ROOT_L} $(OBJECTS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp header/%.h
	@echo "--> Creating object $@"
	@mkdir -p $(OBJDIR)
	@$(GCC) -c $< -o $@  ${ROOT_I} ${ROOT_L}

clean:
	rm -f $(wildcard $(OBJDIR)/*.o) $(BINS)

.PHONY: all clean
