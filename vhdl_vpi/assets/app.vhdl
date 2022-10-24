LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY up_counter IS
    PORT (
        -- Automated Inserted code for VPI_GHDL
        inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
        -- Automated Inserted code for VPI_GHDL

        dummy : OUT STD_LOGIC_VECTOR (7 DOWNTO 0) := "11111111";
        cout : OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
        enable : IN STD_LOGIC := '1';
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC
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
    SIGNAL count : STD_LOGIC_VECTOR (7 DOWNTO 0) := "00000000";
    SIGNAL clk_s : STD_LOGIC;

BEGIN

    clk_s <= clk;

    u_usbport : UsbPort_VPI_GHDL
    PORT MAP(
        -- Automated Inserted code for VPI_GHDL
        inputPort_SW => inputPort_SW,
        outputPort_SW => outputPort_SW,
        -- Automated Inserted code for VPI_GHDL

        outputPort(0) => clk_s,
        outputPort(1) => dummy(1),
        outputPort(2) => dummy(2),
        outputPort(3) => dummy(3),
        outputPort(4) => dummy(4),
        outputPort(5) => dummy(5),
        outputPort(6) => dummy(6),
        outputPort(7) => dummy(7),
        inputPort => count
    );

    PROCESS (clk, reset, enable) BEGIN
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