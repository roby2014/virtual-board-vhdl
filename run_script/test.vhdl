LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY up_counter IS
    PORT (
        -- Automated Inserted code for VPI_GHDL
        inputPort_SW : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        outputPort_SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
        -- Automated Inserted code for VPI_GHDL

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
    SIGNAL zero : STD_LOGIC_VECTOR (7 DOWNTO 0) := "00000000";
    SIGNAL s_enable : STD_LOGIC;

BEGIN

    s_enable <= enable; -- ask teachers why linking s_enable would make it U

    u_usbport : UsbPort_VPI_GHDL
    PORT MAP(
        -- Automated Inserted code for VPI_GHDL
        inputPort_SW => inputPort_SW,
        outputPort_SW => outputPort_SW,
        -- Automated Inserted code for VPI_GHDL

        outputPort => zero,
        inputPort => count
    );

    PROCESS (clk, reset) BEGIN
        IF (reset = '1') THEN
            count <= (OTHERS => '0');
        ELSIF (rising_edge(clk)) THEN
            IF (enable = '1') THEN
                count <= count + 1;
                cout <= count;
            END IF;
        END IF;
    END PROCESS;

END ARCHITECTURE;