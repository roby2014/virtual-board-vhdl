LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY app_tb IS
END app_tb;

ARCHITECTURE test OF app_tb IS
    COMPONENT up_counter PORT (
        cout : OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
        enable : IN STD_LOGIC;
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC
        );
    END COMPONENT;

    SIGNAL cout : STD_LOGIC_VECTOR (7 DOWNTO 0);
    SIGNAL enable, clk, reset : STD_LOGIC;
BEGIN

    up_counter_tb : up_counter PORT MAP(cout => cout, enable => enable, clk => clk, reset => reset);

    PROCESS BEGIN
        enable <= '0';
        clk <= '0';
        reset <= '0';
        enable <= '1';
        WAIT FOR 1 ns;
        ASSERT false REPORT "end";
        WAIT;
    END PROCESS;
END test;