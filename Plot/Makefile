# Compiler and ROOT configurations
ROOT_I = -I`root-config --incdir` -I./header
ROOT_L = `root-config --libs`
GCC = g++

# Compiler flags
CXXFLAGS = -g -std=c++17 $(ROOT_I) -MMD -MP
LDFLAGS = $(ROOT_L)

# Directories
SRCDIR = cpp
OBJDIR = obj

# Source files and object files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OTHER_SOURCES := main.cpp
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(OTHER_SOURCES))
DEPS    := $(OBJECTS:.o=.d)
BINS    := runMain

# Default target
all: $(BINS)

# Linking the executable
$(BINS): $(OBJECTS)
	@echo "--> Creating executable $@"
	@$(GCC) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Ensure OBJDIR exists
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Compiling object files from SRCDIR
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "--> Creating object $@"
	@$(GCC) $(CXXFLAGS) -c $< -o $@

# Compiling object files from root directory (e.g., main.cpp)
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@echo "--> Creating object $@"
	@$(GCC) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d $(BINS)

# Include dependency files
-include $(DEPS)

