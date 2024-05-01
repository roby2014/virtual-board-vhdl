import argparse
import os
import re
import shutil

parser = argparse.ArgumentParser(
    description="Converts a VHDL circuit that uses the default UsbPort implementation (via JTAG) into one that can use the VPI+GHDL one."
)

# TODO: generate a new Makefile / update the old one with the new files

# python3 usb_port_vpi_ghdl.py --vhdl_dir . source.vhd up_counter

parser.add_argument(
    "--vhdl_dir",
    metavar="vhdl_directory",
    type=str,
    default=".",
    help="directory of where UsbPort.vhd and vhdl project files are (default = root of script)",
)
parser.add_argument(
    "top_entity_file",
    metavar="top_entity_file",
    type=str,
    help="top entity file name (ex: counter.vhd)",
)
parser.add_argument(
    "top_entity_name",
    metavar="top_entity_name",
    type=str,
    help="top entity name (ex: counter)",
)

NEW_USB_PORT_NAME = "UsbPort_VPI_GHDL"

USB_PORT_MAP_REGEX = ":[\s|\n]*(UsbPort)[\s|\n]*PORT[\s|\n]*MAP[\s|\n]*\("
USB_PORT_MAP_LINKS = """
-- Automated Inserted code for VPI_GHDL
    inputPort_SW => inputPort_SW,
    outputPort_SW => outputPort_SW,
-- Automated Inserted code for VPI_GHDL
"""

TOP_ENTITY_REGEX = "ENTITY[\s|\n]*({})[\s|\n]*IS[\s|\n]*PORT[\s|\n]*\("
USB_PORT_COMPONENT_REGEX = "COMPONENT[\s|\n]*(UsbPort)[\s|\n]*PORT[\s|\n]*\("
USB_PORT_DECLARATION = """
-- Automated Inserted code for VPI_GHDL
    inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
-- Automated Inserted code for VPI_GHDL
"""

NEW_USB_PORT_FILE_CONTENT = """-- Auto generated by script.
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;

ENTITY {} IS
    PORT (
        inputPort : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
        outputPort : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0)
    );
END ENTITY;

ARCHITECTURE UsbPort OF {} IS
BEGIN
    inputPort_SW <= inputPort;
    outputPort <= outputPort_SW;
END ARCHITECTURE;
""".format(NEW_USB_PORT_NAME, NEW_USB_PORT_NAME)


# get arguments
args = vars(parser.parse_args())
vhdl_dir = str(args["vhdl_dir"])
top_entity_file = str(args["top_entity_file"])
top_entity_name = str(args["top_entity_name"])

#
# check if UsbPort default exists
usb_port_path = vhdl_dir + "/UsbPort.vhd"
if os.path.exists(usb_port_path) == False:
    print("[-] file '{}' does not exist...".format(usb_port_path))
    exit(1)

#
# get top entity file data
top_entity_file_path = vhdl_dir + "/" + top_entity_file
if os.path.exists(top_entity_file_path) == False:
    print("[-] top file entity '{}' does not exist...".format(top_entity_file_path))
    exit(1)

fp = open(top_entity_file_path, "r")
data = fp.read()
fp.close()

#
# add UsbPort_VPI_GHDL signals to the top entity port
match = re.search(
    TOP_ENTITY_REGEX.format(top_entity_name),
    data,
    flags=re.S | re.I,
)
if match == None:
    print("[-] top entity '{}' does not exist...".format(top_entity_name))
    exit(1)

new_top_entity = match.group(0) + USB_PORT_DECLARATION
new_data = re.sub(
    TOP_ENTITY_REGEX.format(top_entity_name),
    new_top_entity,
    data,
    flags=re.S | re.I,
)
print("[+] added {} signals to top entity {}".format(NEW_USB_PORT_NAME, top_entity_name))

#
# Replace the UsbPort component declaration with UsbPort_VPI_GHDL's one
match = re.search(USB_PORT_COMPONENT_REGEX, new_data, flags=re.S | re.I)
if match == None:
    print(
        "[-] top entity '{}' does not have a valid UsbPort port map instantiation...".format(top_entity_name)
    )
    exit(1)

new_component_name = match.group(0).replace(match.group(1), NEW_USB_PORT_NAME)
new_usb_port_component = new_component_name + USB_PORT_DECLARATION
new_data = re.sub(
    USB_PORT_COMPONENT_REGEX,
    new_usb_port_component,
    new_data,
    flags=re.S | re.I,
)
print("[+] old UsbPort component declaration replaced by {}".format(NEW_USB_PORT_NAME))


#
# Find UsbPort port map (component instantiation) and link the new SW signals
match = re.search(USB_PORT_MAP_REGEX, new_data, flags=re.S|re.I)
if match == None:
    print(
        "[-] top entity '{}' does not have a valid UsbPort port map instantiation...".format(
            top_entity_name
        )
    )
    exit(1)

new_usb_port_name = match.group(0).replace(match.group(1), NEW_USB_PORT_NAME)
new_port_map = new_usb_port_name + USB_PORT_MAP_LINKS
new_data = re.sub(
    USB_PORT_MAP_REGEX,
    new_port_map,
    new_data,
    flags=re.S | re.I,
)
print("[+] new UsbPort SW signals linked")

#
# Create new UsbPort file
new_usb_port_path = str(vhdl_dir) + "/{}.vhd".format(NEW_USB_PORT_NAME)
f = open(new_usb_port_path, "w")
f.write(NEW_USB_PORT_FILE_CONTENT)
f.close()
print("[+] {} generated!".format(new_usb_port_path))

#
# Write new top entity generated file
full_path = vhdl_dir + "/" + top_entity_file.replace(".vhd", "_generated.vhd")
fp = open(full_path, "w")
fp.write(new_data)
fp.close()


#
# Move old UsbPort and top entity to a temporary folder
tmp_folder = vhdl_dir + "/tmp"
if not os.path.exists(tmp_folder):
    os.makedirs(tmp_folder)

shutil.move(usb_port_path, tmp_folder)
shutil.move(top_entity_file_path, tmp_folder)