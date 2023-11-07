-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2023 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Michal Blazek <xblaze38@stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan a zacina vykonavat program
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

-- PC
signal pc_data : std_logic_vector(12 downto 0) := (others => '0');
signal pc_dec : std_logic;
signal pc_inc : std_logic;

-- PTR
signal ptr_data : std_logic_vector(12 downto 0) := (others => '0');
signal ptr_inc : std_logic;
signal ptr_dec : std_logic;
signal ptr_rst : std_logic;

-- MX1
signal mx1_sel : std_logic := '0';
signal mx1_out : std_logic_vector(12 downto 0) := (others => '0');

-- MX2
signal mx2_sel : std_logic_vector(1 downto 0) := (others => '0');
signal mx2_out : std_logic_vector(7 downto 0) := (others => '0');

-- FSM
type states is
(
  resetS,
  startS,
  initS,
  fetchS,
  decodeS,

  pcIncPhase0S,
  pcIncPhase1S,
  pcIncPhase2S,
  pcDecPhase0S,
  pcDecPhase1S,
  pcDecPhase2S,

  ptrIncS,
  ptrDecS,

  whileBeginS,
  whileInside0S,
  whileEndS,
  whileSkipS,
  whileSkipBackS,

  writePhase0S,
  writePhase1S,

  readPhase0S,
  readPhase1S,
  readPhase2S,

  haltS
);
signal state : states := resetS;
signal nextState : states;
signal previousState : states;

begin

 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly. 

-- PC
  process (RESET, CLK, pc_inc, pc_dec)
  begin
    if (RESET = '1') then
      pc_data <= (others => '0');
    elsif (rising_edge(CLK)) then
      if (pc_inc = '1') then
        pc_data <= pc_data + 1;
      elsif (pc_dec = '1') then
        pc_data <= pc_data - 1;
      end if;
    end if;
  end process;

-- PTR
  process (RESET, CLK, ptr_inc, ptr_dec, ptr_rst)
  begin
    if (RESET = '1') then
      ptr_data <= (others => '0');
    elsif (rising_edge(CLK)) then
      if (ptr_rst = '1') then
        ptr_data <= (others => '0');
      elsif (ptr_inc = '1') then
        ptr_data <= ptr_data + 1;
      elsif (ptr_dec = '1') then
        ptr_data <= ptr_data - 1;
      end if;
    end if;
  end process;

-- MX1
  process (RESET, CLK, mx1_sel)
  begin
    if (RESET = '1') then
      mx1_out <= (others => '0');
    elsif (rising_edge(CLK)) then
      if (mx1_sel = '0') then
        mx1_out <= ptr_data;
      elsif (mx1_sel = '1') then
        mx1_out <= pc_data;
      end if;
    end if;
  end process;
  DATA_ADDR <= mx1_out;

-- MX2
  process (RESET, CLK, mx2_sel)
  begin
    if (RESET = '1') then
      mx2_out <= (others => '0');
    elsif (rising_edge(CLK)) then
      if (mx2_sel = "00" and IN_VLD = '1') then
        mx2_out <= IN_DATA;
      elsif (mx2_sel = "01") then
        mx2_out <= DATA_RDATA - 1;
      elsif (mx2_sel = "10") then
        mx2_out <= DATA_RDATA + 1;
      else
        -- mx2_out <= (others => '0');
      end if;
    end if;
  end process;
  DATA_WDATA <= mx2_out;

-- FSM present state
  process (RESET, CLK, EN)
  begin
    if (RESET = '1') then
      state <= resetS;
    elsif (rising_edge(CLK)) then
      if (EN = '1') then
        state <= nextState;
      else
        state <= resetS;
      end if;
    end if;
  end process;

-- FSM next state
  process (state, OUT_BUSY, IN_VLD, DATA_RDATA)
  begin
    pc_inc <= '0';
    pc_dec <= '0';
    ptr_inc <= '0';
    ptr_dec <= '0';
    ptr_rst <= '0';
    DATA_EN <= '1';
    DATA_RDWR <= '0';
    OUT_WE <= '0';
    mx1_sel <= '0';
    mx2_sel <= (others => '0');
    IN_REQ <= '0';
    OUT_DATA <= (others => '0');

    case state is
      when resetS => -- RESET
        pc_inc <= '0';
        pc_dec <= '0';
        ptr_inc <= '0';
        ptr_dec <= '0';
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        OUT_WE <= '0';
        mx1_sel <= '0';
        mx2_sel <= (others => '0');
        IN_REQ <= '0';
        DONE <= '0';
        READY <= '0';
        ptr_rst <= '1';
        nextState <= startS;
      when startS => -- IDLE
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        mx1_sel <= '0';
        READY <= '1';
        nextState <= initS;
      when initS => -- INIT
        if (DATA_RDATA = X"40") then
          nextState <= fetchS;
          ptr_dec <= '1';
        else
          ptr_inc <= '1';
          nextState <= initS;
        end if;
      when fetchS => -- FETCH
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        mx1_sel <= '1';
        nextState <= decodeS;

      when decodeS => -- DECODE
        previousState <= fetchS;
        case DATA_RDATA is
          when X"3E" => -- >
            nextState <= ptrIncS;
          when X"3C" => -- <
            nextState <= ptrDecS;
          when X"2B" => -- +
            nextState <= pcIncPhase0S;
          when X"2D" => -- -
            nextState <= pcDecPhase0S;
          when X"5B" => -- [
            nextState <= whileBeginS;
          when X"5D" => -- ]
            nextState <= whileEndS;
          when X"7E" => -- ~
            previousState <= whileSkipS;
            nextState <= whileEndS;
          when X"2E" => -- .
            nextState <= writePhase0S;
          when X"2C" => -- ,
            nextState <= readPhase0S;
          when X"40" => -- @
            nextState <= haltS;
          when others =>
            pc_inc <= '1';
            nextState <= decodeS;
            -- nextState <= haltS;
        end case;

      when ptrIncS => -- >
        ptr_inc <= '1';-- % X"2000"; --! kdyz ukazuju na konec pameti jdu od zacatku
        -- pc_inc <= '1';
        nextState <= fetchS;

      when ptrDecS => -- <
        ptr_dec <= '1';-- % X"2000"; --! kdyz ukazuju na konec pameti jdu od zacatku
        -- pc_inc <= '1';
        nextState <= fetchS;

      when pcIncPhase0S => -- +
        mx1_sel <= '0';
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        nextState <= pcIncPhase1S;
      when pcIncPhase1S =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        mx1_sel <= '0';
        mx2_sel <= "10";
        nextState <= pcIncPhase2S;
      when pcIncPhase2S =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        nextState <= fetchS;

      when pcDecPhase0S => -- -
        mx1_sel <= '0';
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        nextState <= pcDecPhase1S;
      when pcDecPhase1S =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        mx1_sel <= '0';
        mx2_sel <= "01";
        nextState <= pcDecPhase2S;
      when pcDecPhase2S =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        nextState <= fetchS;

      when writePhase0S => -- .
        mx1_sel <= '0';
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        nextState <= writePhase1S;
      when writePhase1S =>
        if (OUT_BUSY = '1') then
          DATA_RDWR <= '0';
          nextState <= writePhase1S;
        else
          DATA_RDWR <= '0';
          OUT_DATA <= DATA_RDATA;
          OUT_WE <= '1';
          -- pc_inc <= '1';
          previousState <= writePhase1S;
          nextState <= fetchS;
        end if;

      when readPhase0S => -- ,
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        IN_REQ <= '1';
        mx2_sel <= "00";
        nextState <= readPhase1S;
      when readPhase1S =>
        if (IN_VLD /= '1') then
          IN_REQ <= '1';
          nextState <= readPhase1S;
        else
          IN_REQ <= '1';
          DATA_EN <= '1';
          DATA_RDWR <= '1';
          -- pc_inc <= '1';
          nextState <= readPhase2S;
        end if;
      when readPhase2S =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        nextState <= fetchS;

      when whileBeginS => -- [
        -- pc_inc <= '1';
        if (previousState = whileSkipBackS) then
          pc_inc <= '1';
        end if;
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        nextState <= whileInside0S;
      when whileInside0S =>
        if (DATA_RDATA = "00000000") then
          mx1_sel <= '1';
          nextState <= whileSkipS;
        else
          mx1_sel <= '0';
          previousState <= whileInside0S;
          nextState <= fetchS;
        end if;

      when whileEndS => -- ]
        if (previousState = whileSkipS) then -- break
          pc_inc <= '1';
          nextState <= fetchS;
        else -- no break
          if (DATA_RDATA /= "00000000") then
            mx1_sel <= '1';
            nextState <= whileSkipBackS;
          else
            nextState <= fetchS;
          end if;
          -- pc_inc <= '1';
        end if;

      when whileSkipS =>
        if (DATA_RDATA = X"5D") then
          nextState <= whileEndS;
        else
          DATA_EN <= '1';
          DATA_RDWR <= '0';
          mx1_sel <= '1';
          pc_inc <= '1';
          nextState <= whileSkipS;
        end if;

      when whileSkipBackS =>
        if (DATA_RDATA = X"5B") then
          pc_inc <= '1';
          previousState <= whileSkipBackS;
          nextState <= whileBeginS;
        else
          DATA_EN <= '1';
          DATA_RDWR <= '0';
          mx1_sel <= '1';
          pc_dec <= '1';
          nextState <= whileSkipBackS;
        end if;

      when haltS => -- HALT
        DONE <= '1';
        nextState <= haltS;
      when others =>
        pc_inc <= '1';
        nextState <= fetchS;
    end case;
  end process;
end behavioral;
