LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY bcd_7segment IS
    PORT (
        BCDin : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
        Seven_Segment : OUT STD_LOGIC_VECTOR (6 DOWNTO 0));
END bcd_7segment;

ARCHITECTURE Behavioral OF bcd_7segment IS

BEGIN

    PROCESS (BCDin)
    BEGIN
        CASE BCDin IS
            WHEN "0000" =>
                Seven_Segment <= "1111110";
            WHEN "0001" =>
                Seven_Segment <= "0110000";
            WHEN "0010" =>
                Seven_Segment <= "1101101";
            WHEN "0011" =>
                Seven_Segment <= "1111001";
            WHEN "0100" =>
                Seven_Segment <= "0110011";
            WHEN "0101" =>
                Seven_Segment <= "1011011";
            WHEN "0110" =>
                Seven_Segment <= "1011111";
            WHEN "0111" =>
                Seven_Segment <= "1110000";
            WHEN "1000" =>
                Seven_Segment <= "1111111";
            WHEN "1001" =>
                Seven_Segment <= "1111011";
            WHEN OTHERS =>
                Seven_Segment <= "0000000";
        END CASE;
    END PROCESS;
    end Behavioral;