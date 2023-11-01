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
-- Program counter
signal pc_data : std_logic_vector(15 downto 0);
signal pc_ld : std_logic;
signal pc_inc : std_logic;
signal pc_mx : std_logic_vector(15 downto 0);
-- Pointer
signal ptr_data : std_logic_vector(12 downto 0);
signal ptr_inc : std_logic;
signal ptr_dec : std_logic;

begin

 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly. 

  process (RESET)
  begin
    if (rising_edge(RESET)) then
      DATA_EN <= '0'; 
      DONE <= '0';
      READY <= '0';
      IN_REQ <= '0';
      OUT_WE <= '0';
    end if;
  end process;

  -- pc: process (RESET, CLK) --! pekna varianta
  -- begin
  --   if (RESET = '1') then
  --     pc_data <= (others => '0');
  --     READY <= '1';
  --   elsif (rising_edge(CLK)) then
  --     if (pc_ld = '1') then
  --       pc_data <= pc_mx;
  --     elsif (pc_inc = '1') then
  --       pc_data <= pc_data + 1;
  --     end if;
  --   end if;
  -- end process;

  -- ptr: process (RESET, CLK)
	-- begin
	-- 	if (RESET = '1') then
	-- 		ptr_data <= (others => '0');
  --     READY <= '1';
	-- 	elsif (rising_edge(CLK)) then
	-- 		if (ptr_inc = '1') then
  --       ptr_data <= ptr_data + 1;
	-- 		elsif (ptr_dec = '1') then
  --       ptr_data <= ptr_data - 1;
	-- 		end if;
	-- 	end if;
	-- end process;
  


  -- pc_mx <= "0000" & ireg_reg(11 downto 0) when pc_mx_sel="00" else DBUS;
  -- ABUS <= pc_reg when (pc_abus = '1') else (others => 'Z');



  -- reset: process (RESET) -- reset -> incializace stavu
  -- begin
  --   if (rising_edge(RESET)) then
  --     DATA_ADDR <= "0000000000000";
  --     READY <= '1';
  --   end if;
  -- end process;

  -- init: process (RESET, EN, CLK) -- inicializace
  -- begin
  --   if (rising_edge(CLK) and RESET = '0' and EN = '0') then
  --     -- cpu vykonava program pri kazde vzestupne hrane CLK
  --   end if;
  -- end process;

end behavioral;

