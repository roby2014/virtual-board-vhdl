LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY up_counter IS
    PORT (
        -- some random signals..
        iSignals : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	    oSignals : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
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
		inputPort(0) => iSignals(0),
		inputPort(1) => iSignals(1),
		inputPort(2) => iSignals(2),
		inputPort(3) => iSignals(3),
		inputPort(4) => iSignals(4),
		inputPort(5) => iSignals(5),
		inputPort(7) => iSignals(7),
		outputPort(0) => oSignals(0),
		outputPort(1) => oSignals(1),
		outputPort(2) => oSignals(2),
		outputPort(3) => oSignals(3),
		outputPort(4) => oSignals(4),
		outputPort(5) => oSignals(5),
		outputPort(6) => oSignals(6)
	);
END ARCHITECTURE;