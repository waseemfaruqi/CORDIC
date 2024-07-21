
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use IEEE.NUMERIC_STD.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity user_logic is
  Port ( 
        K_Alpha             : in std_logic_vector(31 downto 0); --K is MSBs ALpha is LSBs
        carte_YX            : in std_logic_vector(31 downto 0); --Y is upper word X is lower word
        ck, resetn, i_valid : in std_logic;                     
        o_valid             : out std_logic;
        O_carte_YX          : out std_logic_vector(31 downto 0) -- output is Y upper word and X lower word
  );
end user_logic;

architecture Behavioral of user_logic is
COMPONENT cordic_0 IS
  PORT (
    aclk                    : IN STD_LOGIC;
    aresetn                 : IN STD_LOGIC;
    s_axis_phase_tvalid     : IN STD_LOGIC;
    s_axis_phase_tdata      : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
    s_axis_cartesian_tvalid : IN STD_LOGIC;
    s_axis_cartesian_tdata  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    m_axis_dout_tvalid      : OUT STD_LOGIC;
    m_axis_dout_tdata       : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
  );
END COMPONENT;
signal cartes_input_mul_Y, cartes_input_mul_X     : signed(31 downto 0);
signal cartes_input                               : std_logic_vector(31 downto 0);
begin
    cartes_input_mul_Y      <= signed(K_Alpha(31 downto 16)) * signed(carte_YX(31 downto 16));
    cartes_input_mul_X      <= signed(K_Alpha(31 downto 16)) * signed(carte_YX(15 downto 0 ));
    
    cartes_input(15 downto 0)    <= std_logic_vector(cartes_input_mul_X(23 downto 8));
    cartes_input(31 downto 16)   <= std_logic_vector(cartes_input_mul_Y(23 downto 8));
        
    u : cordic_0 
      PORT map(
                aclk                    => ck,
                aresetn                 => resetn,
                s_axis_phase_tvalid     => i_valid,
                s_axis_phase_tdata      => K_Alpha(15 downto 0),
                s_axis_cartesian_tvalid => i_valid,
                s_axis_cartesian_tdata  => cartes_input,
                m_axis_dout_tvalid      => o_valid,
                m_axis_dout_tdata       => O_carte_YX
      );

end Behavioral;
















