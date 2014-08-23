#
#            DMBS Build System
#     Released into the public domain.
#
#  dean [at] fourwalledcubicle [dot] com
#        www.fourwalledcubicle.com
#

DMBS_BUILD_MODULES         += CUTEST
DMBS_BUILD_TARGETS         += test
DMBS_BUILD_MANDATORY_VARS  += TARGET ARCH MCU SRC
DMBS_BUILD_OPTIONAL_VARS   += BOARD OPTIMIZATION C_STANDARD CPP_STANDARD F_CPU C_FLAGS CPP_FLAGS ASM_FLAGS CC_FLAGS LD_FLAGS OBJDIR OBJECT_FILES DEBUG_TYPE DEBUG_LEVEL LINKER_RELAXATIONS COMPILER_PATH
DMBS_BUILD_PROVIDED_VARS   +=
DMBS_BUILD_PROVIDED_MACROS +=

# -----------------------------------------------------------------------------
#               DMBS UnitTesting Makefile Module.
# -----------------------------------------------------------------------------
# DESCRIPTION:
#   Provides a set of targets to build the testing system and execute it
# -----------------------------------------------------------------------------

SHELL = /bin/sh

ERROR_IF_UNSET   ?= $(if $(filter undefined, $(origin $(strip $(1)))), $(error Makefile $(strip $(1)) value not set))
ERROR_IF_EMPTY   ?= $(if $(strip $($(strip $(1)))), , $(error Makefile $(strip $(1)) option cannot be blank))
ERROR_IF_NONBOOL ?= $(if $(filter Y N, $($(strip $(1)))), , $(error Makefile $(strip $(1)) option must be Y or N))

# Default values of optionally user-supplied variables
TEST_BATCH_MAKER      ?= test/make-tests.sh
FIND_COMMAND          ?= find . -name '*.c'
TEST_TARGET           ?= test/AllTests.c

# Output Messages
MSG_TEST_MESSAGE := ' [TEST]    :'

#Build the master test file
test:
	@echo $(MSG_TEST_CMD) Generating meta-testfile:
	@echo ""
	$(TEST_BATCH_MAKER) "`$(FIND_COMMAND)`" > $(TEST_TARGET)

# Phony build targets for this module
.PHONY: test