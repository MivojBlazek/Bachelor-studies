-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Michal Blažek (xblaze38)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       DIN : in std_logic;
       CNT_OF_CLK : in std_logic_vector(4 downto 0);
       CNT_OF_BITS : in std_logic_vector(3 downto 0);
       RX_EN : out std_logic;
       CNT_EN : out std_logic;
       DOUT_VALID : out std_logic
    );
end entity;



architecture behavioral of UART_RX_FSM is
type STATES is (IDLE, START_BIT, DATA_BITS, STOP_BIT);
signal state : STATES := IDLE; --puvodni stav
begin
    RX_EN <= '1' when (state = DATA_BITS) else '0';
    CNT_EN <= '1' when (state = START_BIT or state = DATA_BITS) else '0';
    DOUT_VALID <= '1' when (state = IDLE) else '0';
    process (CLK, RST)
    begin
        if (CLK'event and CLK = '1') then --rising hrana
            if (RST = '1') then --reset -> do puvodniho stavu
                state <= IDLE;
            else
                case state is
                    when IDLE =>
                        if (DIN = '0') then --prechod do stavu START
                            state <= START_BIT;
                        end if;
                    when START_BIT =>
                        if (CNT_OF_CLK = "11000") then --24 -> prechod do stavu cteni dat
                            state <= DATA_BITS;
                        end if;
                    when DATA_BITS =>
                        if (CNT_OF_BITS = "1000") then --8 -> prechod do stavu STOP
                            state <= STOP_BIT;
                        end if;
                    when STOP_BIT =>
                        if (DIN = '1') then --prechod zpet do stavu IDLE
                            state <= IDLE;
                        end if;
                    when others =>
                        null; --pri jinem stavu hod null
                end case;
            end if;
        end if;
    end process;
end architecture;
