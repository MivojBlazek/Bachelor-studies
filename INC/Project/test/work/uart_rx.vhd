-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Matěj Lepeška (xlepes00)

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
    signal count : std_logic_vector(4 downto 0) ;
    signal data_count : std_logic_vector(3 downto 0);
    signal start : std_logic;
    signal data_start : std_logic;
begin
    
    

    process(CLK)
    begin
        if rising_edge(CLK) then
            if start = '1' then
                if count = "10111" then
                    data_count <= data_count + 1;
                    DOUT(0) <= DIN;
                    count <= "00000";
                else   
                    count <= count + 1;
                end if;
                
            else
                count <= "00000";
                data_count <= "0000";
                DOUT <= (others => '0');
            end if;
            if data_start = '1' then
                if data_count = "1000" then
                    data_count <= "0000";
                else
                    
                end if;
                if count = "01111" then
                    count <= "00000";
                    case data_count is
                        when "0000" =>   
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
                            
                    end case;
                    data_count <= data_count + 1;
                end if;
            else    
            
            end if;
            
        end if;
    end process;
    
    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        COUNT => count,
        DATA_COUNT => data_count,
        START => start,
        DATA_START => data_start,
        D_VLD => DOUT_VLD
    );
                      
    

end architecture;
