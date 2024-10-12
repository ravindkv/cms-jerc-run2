# Compiler and standard
#GCC = g++ -std=c++17
GCC = g++ -g -std=c++17

# Directories
SRCDIR = cpp
OBJDIR = obj

# Sources and objects
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
BINS    := runMain

# Include directories
ROOT_I = -I`root-config --incdir` -I./header
CORRECTION_INC = -I$(pwd)./corrlib/include

# Compiler flags
CXXFLAGS = $(ROOT_I) $(CORRECTION_INC) 

# Libraries
ROOT_L = `root-config --libs`
CORRECTION_LIB = -L$(pwd)./corrlib/lib -lcorrectionlib

# Linker flags
LDFLAGS  = $(ROOT_L) $(CORRECTION_LIB)

# Add clang-tidy check
CLANG_TIDY = clang-tidy
#@echo "--> Running clang-tidy on $<"
#@$(CLANG_TIDY) $<  --  -std=c++17 $(CXXFLAGS)
#@$(CLANG_TIDY) $< -fix --  -std=c++17 $(CXXFLAGS) #to fix the warnings

# Build executable
$(BINS): $(OBJECTS) main.cpp
	@echo "--> Creating executable $@"
	@$(GCC) main.cpp $(OBJECTS) -o $@ $(CXXFLAGS) $(LDFLAGS)

# Build object files
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp header/%.h
	@echo "--> Creating object $@"
	@mkdir -p $(OBJDIR)
	@$(GCC) -c $< -o $@ $(CXXFLAGS) $(LDFLAGS)

# Clean up
clean:
	rm -f $(wildcard $(OBJDIR)/*.o) $(BINS)

.PHONY: all clean

