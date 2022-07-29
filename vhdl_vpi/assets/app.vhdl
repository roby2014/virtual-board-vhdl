LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY up_counter IS
    PORT (
        cout : OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
        enable : IN STD_LOGIC;
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC
    );
END ENTITY;

ARCHITECTURE rtl OF up_counter IS
    SIGNAL count : STD_LOGIC_VECTOR (7 DOWNTO 0) := "00000000";
BEGIN
    PROCESS (clk, reset) BEGIN
        IF (reset = '1') THEN
            count <= (OTHERS => '0');
        ELSIF (rising_edge(clk)) THEN
            IF (enable = '1') THEN
                count <= count + 1;
            END IF;
        END IF;
        cout <= count;
    END PROCESS;
END ARCHITECTURE;