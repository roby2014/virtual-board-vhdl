VHDL_FILES = assets/app.vhdl 	# all vhdl files to analyze
EXEC_TOP_UNIT = up_counter 		# top entity name

SRC		= $(shell find ./src/ -name "*.cpp")
INCLUDE = src
LIB		= 
BUILD   = build
TARGET  = virtual_board.vpi

CXX      = g++
CXXFLAGS = -W -Wall -Wextra -std=c++2a -fPIC -pthread
CXXFLAGS += -Wno-write-strings -Wno-missing-field-initializers -Wno-format
LIBRARIES  = -lm -lconfig++

# TODO: better way of doin this
INCLUDEFLAG = -I$(INCLUDE) -I$(INCLUDE)/assignments_cfg

GHDL     = ghdl
GHDLLD   = $(GHDL) --vpi-link $(CXX)

all: $(BUILD)/$(TARGET)

pch: src/pch.hpp
	$(CXX) src/pch.hpp

# TODO: recompile if headers are changed
$(BUILD)/$(TARGET): $(SRC)
	$(GHDLLD) $(CXXFLAGS) $(INCLUDEFLAG) $^ -o $@ $(LIBRARIES)

exec: $(BUILD)/$(TARGET)
	$(GHDL) -a -fsynopsys $(VHDL_FILES)	
	$(GHDL) -e -fsynopsys $(EXEC_TOP_UNIT)
	$(GHDL) -r -fsynopsys $(EXEC_TOP_UNIT) --vpi=./$(BUILD)/$(TARGET)

clean:
	@rm $(BUILD)/*
	@rm -vf work-*.cf
	@rm -vf *.o

