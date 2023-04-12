-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Matěj Lepeška (xlepes00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       COUNT : in std_logic_vector(4 downto 0);
       DATA_COUNT : in std_logic_vector(3 downto 0);
       DIN : in std_logic;
       START : out std_logic;
       DATA_START : out std_logic;
       D_VLD : out std_logic
    );
end entity;



architecture behavioral of UART_RX_FSM is
    type rx_state is (WAIT_START, WAIT_FIRST_DBIT, DATA_END, STOP_BIT, D_VALID);
    signal state : rx_state;
    signal next_state : rx_state;
begin
    
    process (CLK)
    begin
        if rising_edge(CLK) then
            if RST = '1' then
                state <= WAIT_START;
            else
                state <= next_state;
            end if;
        end if;
    end process;
    process (state, DIN, COUNT, DATA_COUNT)
    begin
        next_state <= state;
        case state is  
            when WAIT_START =>
                if DIN = '0' then
                    next_state <= WAIT_FIRST_DBIT;
                end if;
            when WAIT_FIRST_DBIT => 
                if COUNT = "10111" then
                    next_state <= DATA_END;
                end if;
            when DATA_END =>
                if DATA_COUNT = "1000" then
                    next_state <= STOP_BIT;
                end if;
            when STOP_BIT =>
                if COUNT = "01111" then
                    next_state <= D_VALID;
                end if;
            when D_VALID =>
                next_state <= WAIT_START;
        end case;
    end process;

    process (state)
    begin
        START <= '0';
        DATA_START <= '0';
        D_VLD <= '0';
        case state is
            when WAIT_FIRST_DBIT =>
                START <= '1';
            when DATA_END =>
                START <= '1';
                DATA_START <= '1';
            when STOP_BIT =>
                START <= '1';
            when D_VALID =>
                START <= '1';
                D_VLD <= '1';
            when others =>
        end case;
    end process;

end architecture;
