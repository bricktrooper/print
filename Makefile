# ======= IMPORT CONFIGURATION ======= #

include config.mk

# ======= VERIFY CONFIGURATION ======= #

ifeq ($(CC), )
$(error CC was not specified)
endif

ifeq ($(LD), )
$(error LD was not specified)
endif

ifeq ($(MM), )
$(error MM was not specified)
endif

ifeq ($(ART), )
$(error ART was not specified)
endif

ifeq ($(EXE), )
$(error EXE was not specified)
endif

# ======= PROCESS CONFIGURATION ======= #

MMFLAGS += -I .
CCFLAGS += -I .
SRC += $(wildcard *.c)

ifneq ($(DIR), )
VPATH += $(DIR)
MMFLAGS += $(addprefix -I , $(DIR))
CCFLAGS += $(addprefix -I , $(DIR))
SRC += $(wildcard $(addsuffix /*.c, $(DIR)))
endif

OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)

OBJ := $(addprefix $(ART)/, $(OBJ))

# ======= CREATE ARTIFACTS DIRECTORY ======= #

ifneq ($(DIR), )
$(shell mkdir -p $(addprefix $(ART)/, $(DIR)))
else
$(shell mkdir -p $(ART))
endif

# ======= LINK ======= #

$(ART)/$(EXE): $(OBJ)
	@echo "LD   $^"
	@$(LD) $(LDLIBS) $(LDFLAGS) $^ -o $@
	@echo "EXE  $@"

# ======= COMPILE ======= #

$(ART)/%.o: %.c
	@echo "CC   $<"
	@$(CC) $(CCFLAGS) -c $< -o $@

# ======= GENERATE DEPENDENCIES ======= #

$(ART)/%.d: %.c
	@$(MM) $(MMFLAGS) -MM $< -MT $(subst .d,.o,$@) -MF $@

-include $(addprefix $(ART)/, $(DEP))

# ======= CLEAN ======= #

.PHONY:
clean:
	@echo "RM   $(ART)/"
	@rm -rf $(ART)
