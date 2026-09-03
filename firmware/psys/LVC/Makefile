# Board level makefile
# wraps to call to top level make

TEST ?=
# only forward TEST= when set -- an explicit "TEST=" (even empty) on the
# sub-make command line would lock out the top-level Makefile's own default
# logic, since command-line assignments there can't be overridden
TEST_ARG := $(if $(TEST),TEST=$(TEST),)

# call top level make
.PHONY: all flash clean
all flash clean:
	$(MAKE) -C ../.. BOARD=LVC $(TEST_ARG) $@
