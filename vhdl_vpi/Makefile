VHDL_FILES = assets/app.vhdl assets/UsbPort_VPI_GHDL.vhd # all vhdl files to analyze
EXEC_TOP_UNIT = up_counter 		# top entity name

SRC		= $(shell find ./src/ -name "*.cpp")
OBJ		= $(SRC:.cpp=.o )
INCLUDE = src
LIB		= 
BUILD   = build
TARGET  = virtual_board.vpi

CXX      	= g++
CXXFLAGS 	= -W -Wall -Wextra -std=c++2a -fPIC -pthread
CXXFLAGS 	+= -Wno-write-strings -Wno-missing-field-initializers -Wno-format
LIBRARIES  	= -lm -lconfig++ -L/home/roby/seasocks/build/src/main/c/ -lseasocks

# TODO: better way of doing this
# TODO: seasocks on makefile project...
INCLUDEFLAG = -I$(INCLUDE) -I$(INCLUDE)/assignments_cfg -I/home/roby/seasocks/src/main/c/

GHDL     = ghdl
GHDLLD   = $(GHDL) --vpi-link $(CXX)
GHDLC   = $(GHDL) --vpi-compile $(CXX)

all: $(BUILD)/$(TARGET)

%.o: %.cpp
	$(GHDLC) $(CXXFLAGS) $(INCLUDEFLAG) -c $^ -o $@

compile: $(OBJ)

$(BUILD)/$(TARGET): $(OBJ)
	$(GHDLLD) $(CXXFLAGS) $(INCLUDEFLAG) $^ -o $@ $(LIBRARIES)

exec: $(BUILD)/$(TARGET)
	$(GHDL) -a -fsynopsys $(VHDL_FILES)	
	$(GHDL) -e -fsynopsys $(EXEC_TOP_UNIT)
	$(GHDL) -r -fsynopsys $(EXEC_TOP_UNIT) --vpi=./$(BUILD)/$(TARGET)

clean:
	@rm $(BUILD)/*
	@rm -vf work-*.cf
	@rm -vf src/*.o
	@rm -vf src/assignments_cfg/*.o
