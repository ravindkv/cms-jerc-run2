
ROOT_L=`root-config --libs`
ROOT_I=-I`root-config --incdir` -I./header
GCC=g++ -g -std=c++17

# Add clang-tidy check
CLANG_TIDY = clang-tidy
#@echo "--> Running clang-tidy on $<"
#@$(CLANG_TIDY) $<  --  -std=c++17 $(ROOT_I)
#@$(CLANG_TIDY) $< -fix --  -std=c++17 $(ROOT_I)

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
