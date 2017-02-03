# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=release
endif
export config

PROJECTS := ExpertSystem

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

ExpertSystem: 
	@echo "==== Building ExpertSystem ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f ExpertSystem.make

clean:
	@${MAKE} --no-print-directory -C . -f ExpertSystem.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   ExpertSystem"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"

re: clean all