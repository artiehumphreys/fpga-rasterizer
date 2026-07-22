-- Framebuffer scanout TMDS output: fcayci rgb2tmds + a pixel-domain POR.
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tmds_out is
    port (
        pixclk : in std_logic; -- 75 MHz
        serclk : in std_logic; -- 375 MHz (5x)
        sys_rst_n : in std_logic;
        vid_data : in std_logic_vector(23 downto 0); -- from axis_to_video
        vid_active : in std_logic;
        vid_hsync : in std_logic;
        vid_vsync : in std_logic;
        HDMI_CLK : out std_logic;
        HDMI_CLK_N : out std_logic;
        HDMI_TX : out std_logic_vector(2 downto 0);
        HDMI_TX_N : out std_logic_vector(2 downto 0)
    );
end tmds_out;

architecture rtl of tmds_out is
    signal por_cnt : unsigned(7 downto 0) := (others => '0');
    signal por : std_logic := '1';
    signal rst : std_logic;
    signal rgb : std_logic_vector(23 downto 0);
begin
    -- 256-cycle pixel-domain POR. OSERDESE2 latches garbage if it leaves reset
    -- before pixclk/serclk are stable; the counter runs off pixclk so it cannot
    -- expire before pixclk exists (self-gates on clock stability)
    por_proc : process (pixclk)
    begin
        if rising_edge(pixclk) then
            if sys_rst_n = '0' then
                por_cnt <= (others => '0');
                por <= '1';
            elsif por_cnt /= 255 then
                por_cnt <= por_cnt + 1;
                por <= '1';
            else
                por <= '0';
            end if;
        end if;
    end process;
    rst <= por;

    -- Pixel is RGBA little-endian (R=LSB) 
    -- NOTE: Swap if R and B are inverted
    rgb <= vid_data(7 downto 0) & vid_data(15 downto 8) & vid_data(23 downto 16);

    tmds : entity work.rgb2tmds(rtl)
        generic map (SERIES6 => false)
        port map (
            rst => rst,
            pixelclock => pixclk,
            serialclock => serclk,
            video_data => rgb,
            video_active => vid_active,
            hsync => vid_hsync,
            vsync => vid_vsync,
            clk_p => HDMI_CLK,
            clk_n => HDMI_CLK_N,
            data_p => HDMI_TX,
            data_n => HDMI_TX_N
        );
end rtl;
