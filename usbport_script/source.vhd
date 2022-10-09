LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;

ENTITY up_counter IS 
    PORT (
            clk : out STD_LOGIC;
            ce : out STD_LOGIC;
            q   : in STD_LOGIC_VECTOR(3 downto 0)
    );
END ENTITY;

ARCHITECTURE rtl OF up_counter IS
    COMPONENT UsbPort
        PORT (  
            inputPort : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
            outputPort : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
        );
    END COMPONENT;

BEGIN
    -- ....
    u_usbport : UsbPort
    PORT MAP(
        inputPort(0) => q(0),
        inputPort(1) => q(1),
        inputPort(2) => q(2),
        inputPort(3) => q(3),
        inputPort(4) => q(3),
        inputPort(5) => q(3),
        inputPort(6) => q(3),
        inputPort(7) => q(3),
        outputPort(0) => clk,
        outputPort(1) => ce,
        outputPort(2) => ce,
        outputPort(3) => ce,
        outputPort(4) => ce,
        outputPort(5) => ce,
        outputPort(6) => ce,
        outputPort(7) => ce
    );
    
END ARCHITECTURE;