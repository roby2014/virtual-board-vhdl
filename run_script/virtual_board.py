#! /usr/bin/env python

# author: roby
# python script to run the VHDL simulator alongside with the VPI module (virtual_board)

# GOAL:
# 1. replace UsbPort and top entity file if project uses default UsbPort implementation TODO: via other script?
# 1. analyze and elaborate VHDL project files (with GHDL -a & -e)
# 2. compile VPI module (virtual_board)
# 3. run simulation alongside with VPI module

# USAGE: virtual_board vhdl_proj_dir top_entity_name vpi_file_path
# "./virtual_board -h" for more info

import argparse
from genericpath import isfile
import os
from subprocess import CalledProcessError, Popen, PIPE

parser = argparse.ArgumentParser(
    description="script to run the VHDL simulator alongside with the VPI module (virtual_board)"
)

# VHDL project directory
parser.add_argument(
    "vhdl_proj_dir",
    metavar="vhdl_proj_dir",
    type=str,
    help="directory of where VHDL project files are (ex: /home/roby/lic/)",
)

# VHDL top entity name
parser.add_argument(
    "top_entity_name",
    metavar="top_entity_name",
    type=str,
    help="VHDL top entity name"
)

# VHDL top entity name
parser.add_argument(
    "vpi_file_path",
    metavar="vpi_file_path",
    type=str,
    help="VPI module file path (ex: /home/roby/myfiles/virtual_board.vpi"
)


#
# parse arguments
args = vars(parser.parse_args())

#
# loop all VHDL files from <vhdl_directory> and analyze them
vhdl_proj_dir = str(args["vhdl_proj_dir"])

for filename in os.listdir(vhdl_proj_dir):
    file = os.path.join(vhdl_proj_dir, filename)

    if os.path.isfile(file) == False:
        continue

    if (file.endswith(".vhd") == False and file.endswith(".vhdl")) == False:
        continue

    p = Popen(["ghdl", "-a", "-fsynopsys", file], stdout=PIPE, stderr=PIPE)
    output, error = p.communicate()
    if p.returncode != 0: 
        print("[ERROR] ABORTING: Could not analyze '{}':\n{}".format(file, error.decode()))
        exit(1)

#
# elaborate top entity
top_entity_name = str(args["top_entity_name"])

p = Popen(["ghdl", "-e", "-fsynopsys", top_entity_name], stdout=PIPE, stderr=PIPE)
output, error = p.communicate()
if p.returncode != 0: 
    print("[ERROR] ABORTING: Could not elaborate '{}':\n{}".format(top_entity_name, error.decode()))
    exit(1)

#
# compile VPI module with user's compiler
# # # TODO: do we need to compile it?
# # # are shared libraries (.so) portable between machines?

#
# run simulation alongside with the VPI module
vpi_file_path = str(args["vpi_file_path"])
if os.path.isfile(vpi_file_path) == False:
    print("[ERROR] ABORTING: VPI file '{}' does not exist.".format(vpi_file_path))
    exit(1)

# Executes [cmd] program and returns its live output 
def execute(cmd):
    popen = Popen(cmd, stdout=PIPE, universal_newlines=True)
    for stdout_line in iter(popen.stdout.readline, ""):
        yield stdout_line 
    popen.stdout.close()
    return_code = popen.wait()
    if return_code:
        print("[ERROR] ABORTING: Could not run virtual board simulation.\n")
        exit(-1)

for output in execute(["ghdl", "-r", "-fsynopsys", top_entity_name, "--vpi={}".format(vpi_file_path)]):
    print(output, end="")