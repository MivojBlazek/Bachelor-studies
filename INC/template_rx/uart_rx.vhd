-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Michal Blažek (xblaze38)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
signal CNT_OF_CLK : std_logic_vector(4 downto 0);
signal CNT_OF_BITS : std_logic_vector(3 downto 0);
signal RX_EN : std_logic;
signal CNT_EN : std_logic;
signal DOUT_VALID : std_logic;
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        CNT_OF_CLK => CNT_OF_CLK,
        CNT_OF_BITS => CNT_OF_BITS,
        RX_EN => RX_EN,
        CNT_EN => CNT_EN,
        DOUT_VALID => DOUT_VALID
    );

    process (CLK)
    begin
        if (CLK'event and CLK = '1') then --rising hrana
            if (DOUT_VALID = '1') then
                DOUT_VLD <= '1';
                CNT_OF_BITS <= "0000";
            else
                DOUT_VLD <= '0';
            end if;

            if (CNT_EN = '1') then --je povoleno citat
                CNT_OF_CLK <= CNT_OF_CLK + 1;
            else
                CNT_OF_CLK <= "00000";
            end if;

            if (RX_EN = '1') then --je povoleno prijimani
                if (CNT_OF_CLK(4) = '1') then --citani prekrocilo 16 -> smazat a prejit o bit
                    CNT_OF_CLK <= "00000";
                    case CNT_OF_BITS is --urceni bitu, kam se maji zapsat informace
                        when "0000" =>
                            DOUT(0) <= DIN;
                        when "0001" =>
                            DOUT(1) <= DIN;
                        when "0010" =>
                            DOUT(2) <= DIN;
                        when "0011" =>
                            DOUT(3) <= DIN;
                        when "0100" =>
                            DOUT(4) <= DIN;
                        when "0101" =>
                            DOUT(5) <= DIN;
                        when "0110" =>
                            DOUT(6) <= DIN;
                        when "0111" =>
                            DOUT(7) <= DIN;
                        when others =>
                            null;
                    end case;
                    CNT_OF_BITS <= CNT_OF_BITS + 1; --prechod na dalsi bit dat
                end if;
            end if;
        end if;
    end process;


    --DOUT <= (others => '0');
    --DOUT_VLD <= '0';

end architecture;
