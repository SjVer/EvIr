########################################################################
####################### Makefile Template ##############################
########################################################################
# UNIX-based OS variables & settings
RM = rm
MKDIR = mkdir
SHELL := /bin/bash

# Compiler settings - Can be customized.
CC = clang++
LLVMVERSION = 12

MUTE = varargs c99-designator # write-strings sign-compare unused-function comment dangling-gsl unknown-warning-option c++17-extensions
LLVMFLAGS = llvm-config-$(LLVMVERSION) --cxxflags
DEFS = 
CXXFLAGS = -Wall $(addprefix -Wno-,$(MUTE)) $(addprefix -D,$(DEFS)) -fPIC # `$(LLVMFLAGS)`
LDFLAGS = -shared #`$(LLVMFLAGS) --ldflags --system-libs --libs`

# Makefile settings - Can be customized.
APPNAME = libeviir.so
EXT = .cpp
SRCDIR = lib
HEADERDIR = include
BINDIR = bin
OBJDIR = $(BINDIR)/obj

############## Do not change anything from here downwards! #############
SRC = $(shell find $(SRCDIR)/ -type f -name '*$(EXT)')
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
APP = $(BINDIR)/$(APPNAME)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)


PCHS = .base_pch.hpp
PCH_OUT_DIR = $(BINDIR)/pch
PCH_SRC = $(addprefix $(HEADERDIR)/,$(PCHS))
PCH_OUT = $(PCH_SRC:$(HEADERDIR)/%=$(PCH_OUT_DIR)/%.gch)
PCHFLAGS = $(CXXFLAGS) -x c++-header
INC_PCH_FLAG = $(addprefix -include-pch ,$(PCH_OUT))

DEBUGDEFS = -DDEBUG -ggdb

OBJCOUNT_NOPAD = $(shell v=`echo $(OBJ) | wc -w`; echo `seq 1 $$(expr $$v)`)
OBJCOUNT = $(foreach v,$(OBJCOUNT_NOPAD),$(shell printf '%02d' $(v)))


########################################################################
####################### Targets beginning here #########################
########################################################################

.MAIN: $(APP)
all: $(APP)
.DEFAULT_GOAL := $(APP)

# Builds the app
$(APP): $(OBJ) | makedirs
	@printf "[final] compiling final product $(notdir $@)..."
	@$(CC) $(CXXFLAGS) -I$(HEADERDIR)/$(TARGET) -o $@ $^ $(LDFLAGS)
	@printf "\b\b done!\n"

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) | makedirs
	@printf "[$(word 1,$(OBJCOUNT))/$(words $(OBJ))] compiling $(notdir $<) into $(notdir $@)..."
	@$(CC) $(CXXFLAGS) -I$(HEADERDIR)/$(TARGET) $(INC_PCH_FLAG) -I $(HEADERDIR) -o $@ -c $<
	@printf "\b\b done!\n"
	$(eval OBJCOUNT = $(filter-out $(word 1,$(OBJCOUNT)),$(OBJCOUNT)))

pchs: $(PCH_OUT)
$(PCH_OUT_DIR)/%.gch: $(HEADERDIR)/% | makedirs
	@printf "[pchs] compiling $(notdir $<)..."
	@$(CC) $(PCHFLAGS) $^ -o $@
	@printf "\b\b done!\n"

############################################################################

# Cleans complete project
.PHONY: clean
clean:
	@$(RM) -rf $(BINDIR)

.PHONY: makedirs
makedirs:
	@$(MKDIR) -p $(BINDIR)
	@$(MKDIR) -p $(OBJDIR)
	@$(MKDIR) -p $(OBJDIR)/ir
	@$(MKDIR) -p $(PCH_OUT_DIR)

.PHONY: remake
remake: clean $(APP)

############################################################################

.PHONY: test
test: $(APP)
	@printf ">>> COMPILING "
	$(CC) -I$(HEADERDIR) test/test.cpp -o $(BINDIR)/test -L$(BINDIR) -leviir
	@printf ">>> RUNNING "
	bin/test > test/out.eviir
	@printf ">>> DONE "
	cat test/out.eviir

.PHONY: test-debug
test-debug: debug $(APP)
# 	@printf "============ Running \"valgrind $(APP) test/test.evi -o bin/test.ll\" ============\n\n"
# 	@valgrind $(APP) test/test.evi -o bin/test.ll $(args)

############################################################################

.PHONY: printdebug
printdebug:
	@echo "debug mode set!"

debug: CXXFLAGS += $(DEBUGDEFS)
debug: printdebug
debug: $(APP)

.PHONY: debug-no-fold
debug-no-fold: CXXFLAGS += -D DEBUG_NO_FOLD
debug-no-fold: debug

############################################################################

git:
	git add --all
	git commit -m $$(test "$(msg)" && echo '$(msg)' || echo upload)
	git push origin main

newfile:
	@test $(name) || ( echo "basename not given! ('make newfile name=BASENAME')"; false )
	touch $(SRCDIR)/$(name).cpp
	touch $(HEADERDIR)/$(name).hpp

.PHONY: doc
doc:
	@cd doc && doxygen

############################################################################
