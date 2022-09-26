LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY app_tb IS
END app_tb;

ARCHITECTURE test OF app_tb IS
    COMPONENT up_counter PORT (
        -- Automated Inserted code for VPI_GHDL
        inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
        -- Automated Inserted code for VPI_GHDL

        dummy : OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
        cout : OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
        enable : IN STD_LOGIC := '1';
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC
        );
    END COMPONENT;

    SIGNAL cout : STD_LOGIC_VECTOR (7 DOWNTO 0);
    SIGNAL enable, clk, reset : STD_LOGIC;
    SIGNAL dummy, inputPort_SW, outputPort_SW : STD_LOGIC_VECTOR (7 DOWNTO 0);
BEGIN

    up_counter_tb : up_counter PORT MAP(cout => cout, enable => enable, clk => clk, reset => reset, outputPort_SW => outputPort_SW, inputPort_SW => inputPort_SW, dummy => dummy);

    PROCESS BEGIN
        enable <= '0';
        clk <= '0';
        reset <= '0';
        enable <= '1';
        WAIT FOR 1 ns;

        clk <= '1';
        WAIT FOR 1 ns;

        clk <= '0';
        WAIT FOR 1 ns;

        clk <= '1';
        WAIT FOR 1 ns;

        clk <= '0';
        WAIT FOR 1 ns;

        clk <= '1';
        WAIT FOR 1 ns;
        ASSERT false REPORT "end";
        WAIT;
    END PROCESS;
END test;