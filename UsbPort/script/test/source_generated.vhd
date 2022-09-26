LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;

ENTITY up_counter IS
    PORT (
-- Automated Inserted code for VPI_GHDL
    inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
-- Automated Inserted code for VPI_GHDL

        clk : IN STD_LOGIC;
        q : OUT STD_LOGIC
    );
END ENTITY;

ARCHITECTURE rtl OF up_counter IS
    COMPONENT UsbPort_VPI_GHDL
        PORT (
-- Automated Inserted code for VPI_GHDL
    inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
-- Automated Inserted code for VPI_GHDL

            inputPort : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
            outputPort : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
        );
    END COMPONENT;

BEGIN
    u_usbport : UsbPort_VPI_GHDL
    PORT MAP(
-- Automated Inserted code for VPI_GHDL
    inputPort_SW => inputPort_SW,
    outputPort_SW => outputPort_SW,
-- Automated Inserted code for VPI_GHDL

        inputPort(0) => clk,
        inputPort(1) => clk,
        inputPort(2) => clk,
        inputPort(3) => clk,
        inputPort(4) => clk,
        inputPort(5) => clk,
        inputPort(6) => clk,
        inputPort(7) => clk,
        outputPort(0) => q,
        outputPort(1) => q,
        outputPort(2) => q,
        outputPort(3) => q,
        outputPort(4) => q,
        outputPort(5) => q,
        outputPort(6) => q,
        outputPort(7) => q
    );

END ARCHITECTURE;