library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity UART_RX is
  port (
    CLK: in std_logic;
    RST: in std_logic;
    DIN: in std_logic;
    DOUT: out std_logic_vector (7 downto 0);
    DOUT_VLD: out std_logic
  );
end entity;
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart_rx_fsm is
  port (
    clk : in std_logic;
    rst : in std_logic;
    din : in std_logic;
    cnt_of_clk : in std_logic_vector (4 downto 0);
    cnt_of_bits : in std_logic_vector (3 downto 0);
    rx_en : out std_logic;
    cnt_en : out std_logic;
    dout_valid : out std_logic);
end entity uart_rx_fsm;

architecture rtl of uart_rx_fsm is
  signal state : std_logic_vector (1 downto 0);
  signal n77_o : std_logic;
  signal n78_o : std_logic;
  signal n82_o : std_logic;
  signal n84_o : std_logic;
  signal n85_o : std_logic;
  signal n86_o : std_logic;
  signal n90_o : std_logic;
  signal n91_o : std_logic;
  signal n95_o : std_logic;
  signal n96_o : std_logic;
  signal n98_o : std_logic_vector (1 downto 0);
  signal n100_o : std_logic;
  signal n102_o : std_logic;
  signal n104_o : std_logic_vector (1 downto 0);
  signal n106_o : std_logic;
  signal n108_o : std_logic;
  signal n110_o : std_logic_vector (1 downto 0);
  signal n112_o : std_logic;
  signal n114_o : std_logic_vector (1 downto 0);
  signal n116_o : std_logic;
  signal n117_o : std_logic_vector (3 downto 0);
  signal n118_o : std_logic_vector (1 downto 0);
  signal n120_o : std_logic_vector (1 downto 0);
  signal n123_q : std_logic_vector (1 downto 0) := "00";
begin
  rx_en <= n78_o;
  cnt_en <= n86_o;
  dout_valid <= n91_o;
  -- uart_rx_fsm.vhd:27:8
  state <= n123_q; -- (isignal)
  -- uart_rx_fsm.vhd:29:30
  n77_o <= '1' when state = "10" else '0';
  -- uart_rx_fsm.vhd:29:18
  n78_o <= '0' when n77_o = '0' else '1';
  -- uart_rx_fsm.vhd:30:31
  n82_o <= '1' when state = "01" else '0';
  -- uart_rx_fsm.vhd:30:52
  n84_o <= '1' when state = "10" else '0';
  -- uart_rx_fsm.vhd:30:43
  n85_o <= n82_o or n84_o;
  -- uart_rx_fsm.vhd:30:19
  n86_o <= '0' when n85_o = '0' else '1';
  -- uart_rx_fsm.vhd:31:35
  n90_o <= '1' when state = "00" else '0';
  -- uart_rx_fsm.vhd:31:23
  n91_o <= '0' when n90_o = '0' else '1';
  -- uart_rx_fsm.vhd:34:31
  n95_o <= '1' when rising_edge (clk) else '0';
  -- uart_rx_fsm.vhd:40:33
  n96_o <= not din;
  -- uart_rx_fsm.vhd:40:25
  n98_o <= state when n96_o = '0' else "01";
  -- uart_rx_fsm.vhd:39:21
  n100_o <= '1' when state = "00" else '0';
  -- uart_rx_fsm.vhd:44:40
  n102_o <= '1' when cnt_of_clk = "11000" else '0';
  -- uart_rx_fsm.vhd:44:25
  n104_o <= state when n102_o = '0' else "10";
  -- uart_rx_fsm.vhd:43:21
  n106_o <= '1' when state = "01" else '0';
  -- uart_rx_fsm.vhd:48:41
  n108_o <= '1' when cnt_of_bits = "1000" else '0';
  -- uart_rx_fsm.vhd:48:25
  n110_o <= state when n108_o = '0' else "11";
  -- uart_rx_fsm.vhd:47:21
  n112_o <= '1' when state = "10" else '0';
  -- uart_rx_fsm.vhd:52:25
  n114_o <= state when din = '0' else "00";
  -- uart_rx_fsm.vhd:51:21
  n116_o <= '1' when state = "11" else '0';
  n117_o <= n116_o & n112_o & n106_o & n100_o;
  -- uart_rx_fsm.vhd:38:17
  with n117_o select n118_o <=
    n114_o when "1000",
    n110_o when "0100",
    n104_o when "0010",
    n98_o when "0001",
    state when others;
  -- uart_rx_fsm.vhd:35:13
  n120_o <= n118_o when rst = '0' else "00";
  -- uart_rx_fsm.vhd:34:9
  process (clk)
  begin
    if rising_edge (clk) then
      n123_q <= n120_o;
    end if;
  end process;
end rtl;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

architecture rtl of uart_rx is
  signal wrap_CLK: std_logic;
  signal wrap_RST: std_logic;
  signal wrap_DIN: std_logic;
  subtype typwrap_DOUT is std_logic_vector (7 downto 0);
  signal wrap_DOUT: typwrap_DOUT;
  signal wrap_DOUT_VLD: std_logic;
  signal cnt_of_clk : std_logic_vector (4 downto 0);
  signal cnt_of_bits : std_logic_vector (3 downto 0);
  signal rx_en : std_logic;
  signal cnt_en : std_logic;
  signal dout_valid : std_logic;
  signal fsm_rx_en : std_logic;
  signal fsm_cnt_en : std_logic;
  signal fsm_dout_valid : std_logic;
  signal n7_o : std_logic;
  signal n10_o : std_logic;
  signal n12_o : std_logic_vector (3 downto 0);
  signal n14_o : std_logic_vector (4 downto 0);
  signal n16_o : std_logic_vector (4 downto 0);
  signal n17_o : std_logic;
  signal n19_o : std_logic;
  signal n21_o : std_logic;
  signal n23_o : std_logic;
  signal n25_o : std_logic;
  signal n27_o : std_logic;
  signal n29_o : std_logic;
  signal n31_o : std_logic;
  signal n33_o : std_logic;
  signal n34_o : std_logic_vector (7 downto 0);
  signal n35_o : std_logic;
  signal n36_o : std_logic;
  signal n37_o : std_logic;
  signal n38_o : std_logic;
  signal n39_o : std_logic;
  signal n40_o : std_logic;
  signal n41_o : std_logic;
  signal n42_o : std_logic;
  signal n43_o : std_logic;
  signal n44_o : std_logic;
  signal n45_o : std_logic;
  signal n46_o : std_logic;
  signal n47_o : std_logic;
  signal n48_o : std_logic;
  signal n49_o : std_logic;
  signal n50_o : std_logic;
  signal n52_o : std_logic_vector (3 downto 0);
  signal n53_o : std_logic_vector (7 downto 0);
  signal n56_o : std_logic_vector (4 downto 0);
  signal n57_o : std_logic_vector (3 downto 0);
  signal n58_o : std_logic;
  signal n59_o : std_logic;
  signal n60_o : std_logic;
  signal n66_o : std_logic_vector (7 downto 0);
  signal n67_q : std_logic_vector (7 downto 0);
  signal n68_q : std_logic;
  signal n69_q : std_logic_vector (4 downto 0);
  signal n70_q : std_logic_vector (3 downto 0);
begin
  wrap_clk <= clk;
  wrap_rst <= rst;
  wrap_din <= din;
  dout <= wrap_dout;
  dout_vld <= wrap_dout_vld;
  wrap_DOUT <= n67_q;
  wrap_DOUT_VLD <= n68_q;
  -- uart_rx.vhd:25:8
  cnt_of_clk <= n69_q; -- (signal)
  -- uart_rx.vhd:26:8
  cnt_of_bits <= n70_q; -- (signal)
  -- uart_rx.vhd:27:8
  rx_en <= fsm_rx_en; -- (signal)
  -- uart_rx.vhd:28:8
  cnt_en <= fsm_cnt_en; -- (signal)
  -- uart_rx.vhd:29:8
  dout_valid <= fsm_dout_valid; -- (signal)
  -- uart_rx.vhd:33:5
  fsm : entity work.uart_rx_fsm port map (
    clk => wrap_CLK,
    rst => wrap_RST,
    din => wrap_DIN,
    cnt_of_clk => cnt_of_clk,
    cnt_of_bits => cnt_of_bits,
    rx_en => fsm_rx_en,
    cnt_en => fsm_cnt_en,
    dout_valid => fsm_dout_valid);
  -- uart_rx.vhd:47:31
  n7_o <= '1' when rising_edge (wrap_CLK) else '0';
  -- uart_rx.vhd:48:13
  n10_o <= '0' when dout_valid = '0' else '1';
  -- uart_rx.vhd:48:13
  n12_o <= cnt_of_bits when dout_valid = '0' else "0000";
  -- uart_rx.vhd:56:42
  n14_o <= std_logic_vector (unsigned (cnt_of_clk) + unsigned'("00001"));
  -- uart_rx.vhd:55:13
  n16_o <= "00000" when cnt_en = '0' else n14_o;
  -- uart_rx.vhd:62:31
  n17_o <= cnt_of_clk (4);
  -- uart_rx.vhd:65:25
  n19_o <= '1' when cnt_of_bits = "0000" else '0';
  -- uart_rx.vhd:67:25
  n21_o <= '1' when cnt_of_bits = "0001" else '0';
  -- uart_rx.vhd:69:25
  n23_o <= '1' when cnt_of_bits = "0010" else '0';
  -- uart_rx.vhd:71:25
  n25_o <= '1' when cnt_of_bits = "0011" else '0';
  -- uart_rx.vhd:73:25
  n27_o <= '1' when cnt_of_bits = "0100" else '0';
  -- uart_rx.vhd:75:25
  n29_o <= '1' when cnt_of_bits = "0101" else '0';
  -- uart_rx.vhd:77:25
  n31_o <= '1' when cnt_of_bits = "0110" else '0';
  -- uart_rx.vhd:79:25
  n33_o <= '1' when cnt_of_bits = "0111" else '0';
  n34_o <= n33_o & n31_o & n29_o & n27_o & n25_o & n23_o & n21_o & n19_o;
  n35_o <= n67_q (0);
  -- uart_rx.vhd:64:21
  with n34_o select n36_o <=
    n35_o when "10000000",
    n35_o when "01000000",
    n35_o when "00100000",
    n35_o when "00010000",
    n35_o when "00001000",
    n35_o when "00000100",
    n35_o when "00000010",
    wrap_DIN when "00000001",
    n35_o when others;
  n37_o <= n67_q (1);
  -- uart_rx.vhd:64:21
  with n34_o select n38_o <=
    n37_o when "10000000",
    n37_o when "01000000",
    n37_o when "00100000",
    n37_o when "00010000",
    n37_o when "00001000",
    n37_o when "00000100",
    wrap_DIN when "00000010",
    n37_o when "00000001",
    n37_o when others;
  n39_o <= n67_q (2);
  -- uart_rx.vhd:64:21
  with n34_o select n40_o <=
    n39_o when "10000000",
    n39_o when "01000000",
    n39_o when "00100000",
    n39_o when "00010000",
    n39_o when "00001000",
    wrap_DIN when "00000100",
    n39_o when "00000010",
    n39_o when "00000001",
    n39_o when others;
  n41_o <= n67_q (3);
  -- uart_rx.vhd:64:21
  with n34_o select n42_o <=
    n41_o when "10000000",
    n41_o when "01000000",
    n41_o when "00100000",
    n41_o when "00010000",
    wrap_DIN when "00001000",
    n41_o when "00000100",
    n41_o when "00000010",
    n41_o when "00000001",
    n41_o when others;
  n43_o <= n67_q (4);
  -- uart_rx.vhd:64:21
  with n34_o select n44_o <=
    n43_o when "10000000",
    n43_o when "01000000",
    n43_o when "00100000",
    wrap_DIN when "00010000",
    n43_o when "00001000",
    n43_o when "00000100",
    n43_o when "00000010",
    n43_o when "00000001",
    n43_o when others;
  n45_o <= n67_q (5);
  -- uart_rx.vhd:64:21
  with n34_o select n46_o <=
    n45_o when "10000000",
    n45_o when "01000000",
    wrap_DIN when "00100000",
    n45_o when "00010000",
    n45_o when "00001000",
    n45_o when "00000100",
    n45_o when "00000010",
    n45_o when "00000001",
    n45_o when others;
  n47_o <= n67_q (6);
  -- uart_rx.vhd:64:21
  with n34_o select n48_o <=
    n47_o when "10000000",
    wrap_DIN when "01000000",
    n47_o when "00100000",
    n47_o when "00010000",
    n47_o when "00001000",
    n47_o when "00000100",
    n47_o when "00000010",
    n47_o when "00000001",
    n47_o when others;
  n49_o <= n67_q (7);
  -- uart_rx.vhd:64:21
  with n34_o select n50_o <=
    wrap_DIN when "10000000",
    n49_o when "01000000",
    n49_o when "00100000",
    n49_o when "00010000",
    n49_o when "00001000",
    n49_o when "00000100",
    n49_o when "00000010",
    n49_o when "00000001",
    n49_o when others;
  -- uart_rx.vhd:84:48
  n52_o <= std_logic_vector (unsigned (cnt_of_bits) + unsigned'("0001"));
  n53_o <= n50_o & n48_o & n46_o & n44_o & n42_o & n40_o & n38_o & n36_o;
  -- uart_rx.vhd:61:13
  n56_o <= n16_o when n59_o = '0' else "00000";
  -- uart_rx.vhd:61:13
  n57_o <= n12_o when n60_o = '0' else n52_o;
  -- uart_rx.vhd:61:13
  n58_o <= rx_en and n17_o;
  -- uart_rx.vhd:61:13
  n59_o <= rx_en and n17_o;
  -- uart_rx.vhd:61:13
  n60_o <= rx_en and n17_o;
  -- uart_rx.vhd:61:13
  n66_o <= n67_q when n58_o = '0' else n53_o;
  -- uart_rx.vhd:47:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n67_q <= n66_o;
    end if;
  end process;
  -- uart_rx.vhd:47:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n68_q <= n10_o;
    end if;
  end process;
  -- uart_rx.vhd:47:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n69_q <= n56_o;
    end if;
  end process;
  -- uart_rx.vhd:47:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n70_q <= n57_o;
    end if;
  end process;
end rtl;
