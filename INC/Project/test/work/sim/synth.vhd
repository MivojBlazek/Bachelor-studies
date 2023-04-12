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
    count : in std_logic_vector (4 downto 0);
    data_count : in std_logic_vector (3 downto 0);
    din : in std_logic;
    start : out std_logic;
    data_start : out std_logic;
    d_vld : out std_logic);
end entity uart_rx_fsm;

architecture rtl of uart_rx_fsm is
  signal state : std_logic_vector (2 downto 0);
  signal next_state : std_logic_vector (2 downto 0);
  signal n102_o : std_logic;
  signal n104_o : std_logic_vector (2 downto 0);
  signal n107_q : std_logic_vector (2 downto 0);
  signal n110_o : std_logic;
  signal n112_o : std_logic_vector (2 downto 0);
  signal n114_o : std_logic;
  signal n116_o : std_logic;
  signal n118_o : std_logic_vector (2 downto 0);
  signal n120_o : std_logic;
  signal n122_o : std_logic;
  signal n124_o : std_logic_vector (2 downto 0);
  signal n126_o : std_logic;
  signal n128_o : std_logic;
  signal n130_o : std_logic_vector (2 downto 0);
  signal n132_o : std_logic;
  signal n134_o : std_logic;
  signal n135_o : std_logic_vector (4 downto 0);
  signal n138_o : std_logic_vector (2 downto 0);
  signal n143_o : std_logic;
  signal n145_o : std_logic;
  signal n147_o : std_logic;
  signal n149_o : std_logic;
  signal n150_o : std_logic_vector (3 downto 0);
  signal n156_o : std_logic;
  signal n160_o : std_logic;
  signal n164_o : std_logic;
begin
  start <= n156_o;
  data_start <= n160_o;
  d_vld <= n164_o;
  -- work/uart_rx_fsm.vhd:27:12
  state <= n107_q; -- (signal)
  -- work/uart_rx_fsm.vhd:28:12
  next_state <= n138_o; -- (signal)
  -- work/uart_rx_fsm.vhd:33:12
  n102_o <= '1' when rising_edge (clk) else '0';
  -- work/uart_rx_fsm.vhd:34:13
  n104_o <= next_state when rst = '0' else "000";
  -- work/uart_rx_fsm.vhd:33:9
  process (clk)
  begin
    if rising_edge (clk) then
      n107_q <= n104_o;
    end if;
  end process;
  -- work/uart_rx_fsm.vhd:46:24
  n110_o <= not din;
  -- work/uart_rx_fsm.vhd:46:17
  n112_o <= state when n110_o = '0' else "001";
  -- work/uart_rx_fsm.vhd:45:13
  n114_o <= '1' when state = "000" else '0';
  -- work/uart_rx_fsm.vhd:50:26
  n116_o <= '1' when count = "10111" else '0';
  -- work/uart_rx_fsm.vhd:50:17
  n118_o <= state when n116_o = '0' else "010";
  -- work/uart_rx_fsm.vhd:49:13
  n120_o <= '1' when state = "001" else '0';
  -- work/uart_rx_fsm.vhd:54:31
  n122_o <= '1' when data_count = "1000" else '0';
  -- work/uart_rx_fsm.vhd:54:17
  n124_o <= state when n122_o = '0' else "011";
  -- work/uart_rx_fsm.vhd:53:13
  n126_o <= '1' when state = "010" else '0';
  -- work/uart_rx_fsm.vhd:58:26
  n128_o <= '1' when count = "01111" else '0';
  -- work/uart_rx_fsm.vhd:58:17
  n130_o <= state when n128_o = '0' else "100";
  -- work/uart_rx_fsm.vhd:57:13
  n132_o <= '1' when state = "011" else '0';
  -- work/uart_rx_fsm.vhd:61:13
  n134_o <= '1' when state = "100" else '0';
  n135_o <= n134_o & n132_o & n126_o & n120_o & n114_o;
  -- work/uart_rx_fsm.vhd:44:9
  with n135_o select n138_o <=
    "000" when "10000",
    n130_o when "01000",
    n124_o when "00100",
    n118_o when "00010",
    n112_o when "00001",
    "XXX" when others;
  -- work/uart_rx_fsm.vhd:72:13
  n143_o <= '1' when state = "001" else '0';
  -- work/uart_rx_fsm.vhd:74:13
  n145_o <= '1' when state = "010" else '0';
  -- work/uart_rx_fsm.vhd:77:13
  n147_o <= '1' when state = "011" else '0';
  -- work/uart_rx_fsm.vhd:79:13
  n149_o <= '1' when state = "100" else '0';
  n150_o <= n149_o & n147_o & n145_o & n143_o;
  -- work/uart_rx_fsm.vhd:71:9
  with n150_o select n156_o <=
    '1' when "1000",
    '1' when "0100",
    '1' when "0010",
    '1' when "0001",
    '0' when others;
  -- work/uart_rx_fsm.vhd:71:9
  with n150_o select n160_o <=
    '0' when "1000",
    '0' when "0100",
    '1' when "0010",
    '0' when "0001",
    '0' when others;
  -- work/uart_rx_fsm.vhd:71:9
  with n150_o select n164_o <=
    '1' when "1000",
    '0' when "0100",
    '0' when "0010",
    '0' when "0001",
    '0' when others;
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
  signal count : std_logic_vector (4 downto 0);
  signal data_count : std_logic_vector (3 downto 0);
  signal start : std_logic;
  signal data_start : std_logic;
  signal n4_o : std_logic;
  signal n6_o : std_logic;
  signal n8_o : std_logic_vector (3 downto 0);
  signal n10_o : std_logic_vector (4 downto 0);
  signal n11_o : std_logic;
  signal n12_o : std_logic;
  signal n14_o : std_logic_vector (4 downto 0);
  signal n15_o : std_logic_vector (3 downto 0);
  signal n17_o : std_logic;
  constant n18_o : std_logic_vector (6 downto 0) := "0000000";
  signal n19_o : std_logic_vector (6 downto 0);
  signal n20_o : std_logic_vector (6 downto 0);
  signal n22_o : std_logic_vector (4 downto 0);
  signal n24_o : std_logic_vector (3 downto 0);
  signal n26_o : std_logic;
  signal n28_o : std_logic_vector (3 downto 0);
  signal n30_o : std_logic;
  signal n32_o : std_logic;
  signal n34_o : std_logic;
  signal n36_o : std_logic;
  signal n38_o : std_logic;
  signal n40_o : std_logic;
  signal n42_o : std_logic;
  signal n44_o : std_logic;
  signal n46_o : std_logic;
  signal n47_o : std_logic_vector (7 downto 0);
  signal n48_o : std_logic;
  signal n49_o : std_logic;
  signal n50_o : std_logic;
  signal n51_o : std_logic;
  signal n52_o : std_logic;
  signal n53_o : std_logic;
  signal n54_o : std_logic;
  signal n55_o : std_logic;
  signal n56_o : std_logic;
  signal n57_o : std_logic;
  signal n58_o : std_logic;
  signal n59_o : std_logic;
  signal n60_o : std_logic;
  signal n61_o : std_logic;
  signal n62_o : std_logic;
  signal n63_o : std_logic;
  signal n64_o : std_logic;
  signal n65_o : std_logic;
  signal n66_o : std_logic;
  signal n67_o : std_logic;
  signal n68_o : std_logic;
  signal n69_o : std_logic;
  signal n70_o : std_logic;
  signal n71_o : std_logic;
  signal n72_o : std_logic;
  signal n73_o : std_logic;
  signal n74_o : std_logic;
  signal n75_o : std_logic;
  signal n77_o : std_logic_vector (3 downto 0);
  signal n78_o : std_logic_vector (6 downto 0);
  signal n79_o : std_logic_vector (6 downto 0);
  signal n81_o : std_logic_vector (4 downto 0);
  signal n82_o : std_logic_vector (3 downto 0);
  signal n83_o : std_logic;
  signal n84_o : std_logic;
  signal n85_o : std_logic_vector (3 downto 0);
  signal n86_o : std_logic_vector (7 downto 0);
  signal n91_q : std_logic_vector (7 downto 0);
  signal n92_q : std_logic_vector (4 downto 0);
  signal n93_q : std_logic_vector (3 downto 0);
  signal fsm_start : std_logic;
  signal fsm_data_start : std_logic;
  signal fsm_d_vld : std_logic;
begin
  wrap_clk <= clk;
  wrap_rst <= rst;
  wrap_din <= din;
  dout <= wrap_dout;
  dout_vld <= wrap_dout_vld;
  wrap_DOUT <= n91_q;
  wrap_DOUT_VLD <= fsm_d_vld;
  -- work/uart_rx.vhd:25:12
  count <= n92_q; -- (signal)
  -- work/uart_rx.vhd:26:12
  data_count <= n93_q; -- (signal)
  -- work/uart_rx.vhd:27:12
  start <= fsm_start; -- (signal)
  -- work/uart_rx.vhd:28:12
  data_start <= fsm_data_start; -- (signal)
  -- work/uart_rx.vhd:35:12
  n4_o <= '1' when rising_edge (wrap_CLK) else '0';
  -- work/uart_rx.vhd:37:26
  n6_o <= '1' when count = "10111" else '0';
  -- work/uart_rx.vhd:38:46
  n8_o <= std_logic_vector (unsigned (data_count) + unsigned'("0001"));
  -- work/uart_rx.vhd:42:36
  n10_o <= std_logic_vector (unsigned (count) + unsigned'("00001"));
  n11_o <= n91_q (0);
  -- work/uart_rx.vhd:37:17
  n12_o <= n11_o when n6_o = '0' else wrap_DIN;
  -- work/uart_rx.vhd:37:17
  n14_o <= n10_o when n6_o = '0' else "00000";
  -- work/uart_rx.vhd:37:17
  n15_o <= data_count when n6_o = '0' else n8_o;
  -- work/uart_rx.vhd:36:13
  n17_o <= '0' when start = '0' else n12_o;
  n19_o <= n91_q (7 downto 1);
  -- work/uart_rx.vhd:36:13
  n20_o <= "0000000" when start = '0' else n19_o;
  -- work/uart_rx.vhd:36:13
  n22_o <= "00000" when start = '0' else n14_o;
  -- work/uart_rx.vhd:36:13
  n24_o <= "0000" when start = '0' else n15_o;
  -- work/uart_rx.vhd:51:31
  n26_o <= '1' when data_count = "1000" else '0';
  -- work/uart_rx.vhd:51:17
  n28_o <= n24_o when n26_o = '0' else "0000";
  -- work/uart_rx.vhd:56:26
  n30_o <= '1' when count = "01111" else '0';
  -- work/uart_rx.vhd:59:25
  n32_o <= '1' when data_count = "0000" else '0';
  -- work/uart_rx.vhd:60:25
  n34_o <= '1' when data_count = "0001" else '0';
  -- work/uart_rx.vhd:62:25
  n36_o <= '1' when data_count = "0010" else '0';
  -- work/uart_rx.vhd:64:25
  n38_o <= '1' when data_count = "0011" else '0';
  -- work/uart_rx.vhd:66:25
  n40_o <= '1' when data_count = "0100" else '0';
  -- work/uart_rx.vhd:68:25
  n42_o <= '1' when data_count = "0101" else '0';
  -- work/uart_rx.vhd:70:25
  n44_o <= '1' when data_count = "0110" else '0';
  -- work/uart_rx.vhd:72:25
  n46_o <= '1' when data_count = "0111" else '0';
  n47_o <= n46_o & n44_o & n42_o & n40_o & n38_o & n36_o & n34_o & n32_o;
  n48_o <= n91_q (1);
  n49_o <= n18_o (0);
  -- work/uart_rx.vhd:36:13
  n50_o <= n49_o when start = '0' else n48_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n51_o <=
    n50_o when "10000000",
    n50_o when "01000000",
    n50_o when "00100000",
    n50_o when "00010000",
    n50_o when "00001000",
    n50_o when "00000100",
    wrap_DIN when "00000010",
    n50_o when "00000001",
    n50_o when others;
  n52_o <= n91_q (2);
  n53_o <= n18_o (1);
  -- work/uart_rx.vhd:36:13
  n54_o <= n53_o when start = '0' else n52_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n55_o <=
    n54_o when "10000000",
    n54_o when "01000000",
    n54_o when "00100000",
    n54_o when "00010000",
    n54_o when "00001000",
    wrap_DIN when "00000100",
    n54_o when "00000010",
    n54_o when "00000001",
    n54_o when others;
  n56_o <= n91_q (3);
  n57_o <= n18_o (2);
  -- work/uart_rx.vhd:36:13
  n58_o <= n57_o when start = '0' else n56_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n59_o <=
    n58_o when "10000000",
    n58_o when "01000000",
    n58_o when "00100000",
    n58_o when "00010000",
    wrap_DIN when "00001000",
    n58_o when "00000100",
    n58_o when "00000010",
    n58_o when "00000001",
    n58_o when others;
  n60_o <= n91_q (4);
  n61_o <= n18_o (3);
  -- work/uart_rx.vhd:36:13
  n62_o <= n61_o when start = '0' else n60_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n63_o <=
    n62_o when "10000000",
    n62_o when "01000000",
    n62_o when "00100000",
    wrap_DIN when "00010000",
    n62_o when "00001000",
    n62_o when "00000100",
    n62_o when "00000010",
    n62_o when "00000001",
    n62_o when others;
  n64_o <= n91_q (5);
  n65_o <= n18_o (4);
  -- work/uart_rx.vhd:36:13
  n66_o <= n65_o when start = '0' else n64_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n67_o <=
    n66_o when "10000000",
    n66_o when "01000000",
    wrap_DIN when "00100000",
    n66_o when "00010000",
    n66_o when "00001000",
    n66_o when "00000100",
    n66_o when "00000010",
    n66_o when "00000001",
    n66_o when others;
  n68_o <= n91_q (6);
  n69_o <= n18_o (5);
  -- work/uart_rx.vhd:36:13
  n70_o <= n69_o when start = '0' else n68_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n71_o <=
    n70_o when "10000000",
    wrap_DIN when "01000000",
    n70_o when "00100000",
    n70_o when "00010000",
    n70_o when "00001000",
    n70_o when "00000100",
    n70_o when "00000010",
    n70_o when "00000001",
    n70_o when others;
  n72_o <= n91_q (7);
  n73_o <= n18_o (6);
  -- work/uart_rx.vhd:36:13
  n74_o <= n73_o when start = '0' else n72_o;
  -- work/uart_rx.vhd:58:21
  with n47_o select n75_o <=
    wrap_DIN when "10000000",
    n74_o when "01000000",
    n74_o when "00100000",
    n74_o when "00010000",
    n74_o when "00001000",
    n74_o when "00000100",
    n74_o when "00000010",
    n74_o when "00000001",
    n74_o when others;
  -- work/uart_rx.vhd:77:46
  n77_o <= std_logic_vector (unsigned (data_count) + unsigned'("0001"));
  n78_o <= n75_o & n71_o & n67_o & n63_o & n59_o & n55_o & n51_o;
  -- work/uart_rx.vhd:50:13
  n79_o <= n20_o when n83_o = '0' else n78_o;
  -- work/uart_rx.vhd:50:13
  n81_o <= n22_o when n84_o = '0' else "00000";
  -- work/uart_rx.vhd:56:17
  n82_o <= n28_o when n30_o = '0' else n77_o;
  -- work/uart_rx.vhd:50:13
  n83_o <= data_start and n30_o;
  -- work/uart_rx.vhd:50:13
  n84_o <= data_start and n30_o;
  -- work/uart_rx.vhd:50:13
  n85_o <= n24_o when data_start = '0' else n82_o;
  n86_o <= n79_o & n17_o;
  -- work/uart_rx.vhd:35:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n91_q <= n86_o;
    end if;
  end process;
  -- work/uart_rx.vhd:35:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n92_q <= n81_o;
    end if;
  end process;
  -- work/uart_rx.vhd:35:9
  process (wrap_CLK)
  begin
    if rising_edge (wrap_CLK) then
      n93_q <= n85_o;
    end if;
  end process;
  -- work/uart_rx.vhd:87:5
  fsm : entity work.uart_rx_fsm port map (
    clk => wrap_CLK,
    rst => wrap_RST,
    count => count,
    data_count => data_count,
    din => wrap_DIN,
    start => fsm_start,
    data_start => fsm_data_start,
    d_vld => fsm_d_vld);
end rtl;
