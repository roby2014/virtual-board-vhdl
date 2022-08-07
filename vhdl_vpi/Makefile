TARGET  = virtual_board.vpi
VHDL_FILES = assets/app.vhdl # all vhdl files to analyze
EXEC_TOP_UNIT = up_counter # top entity name
BUILD   = build

SRC += src/main.cpp
SRC += src/vpi.cpp
SRC += src/qsf_parser.cpp
SRC += src/virtual_board.cpp
SRC += src/board_config.cpp
SRC += src/utils.cpp

CFLAGS  += -Isrc -O2 -std=c++2a -fPIC -W -Wall -Wno-write-strings -Wno-missing-field-initializers 
LDFLAGS += -lm -lconfig++
GHDL     = ghdl
CXX      = g++
GHDLLD   = $(GHDL) --vpi-link $(CXX)

all: $(BUILD)/$(TARGET)

# TODO: recompile if headers are changed
$(BUILD)/$(TARGET): $(SRC)
	$(GHDLLD) $(CFLAGS) $^ -o $@ $(LDFLAGS)

exec: $(BUILD)/$(TARGET)
	$(GHDL) -a -fsynopsys $(VHDL_FILES)
	$(GHDL) -e -fsynopsys $(EXEC_TOP_UNIT)
	$(GHDL) -r -fsynopsys $(EXEC_TOP_UNIT) --vpi=./$(BUILD)/$(TARGET)

clean:
	@rm -rvf $(BUILD)
	@rm -vf work-*.cf
	@rm -vf *.o
	@rm -vf test
	@rm -vf *.vcd
	@rm -vf trace.txt

