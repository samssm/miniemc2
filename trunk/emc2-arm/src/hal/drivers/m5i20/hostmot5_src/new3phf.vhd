library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity new3ph is
    Port (
	 		clk: in STD_LOGIC;
			pwmrefcnt: in STD_LOGIC_VECTOR(9 downto 0);
			ibus: in STD_LOGIC_VECTOR (15 downto 0);
			obus: out STD_LOGIC_VECTOR (15 downto 0);
			writestb: in STD_LOGIC;
			readstb: in STD_LOGIC;
			addr: in STD_LOGIC_VECTOR (1 downto 0);						
			outa: out STD_LOGIC;
			outb: out STD_LOGIC;
			outc: out STD_LOGIC
			);
end new3ph;

architecture behavioral of new3ph is

signal creg: STD_LOGIC_VECTOR (2 downto 0);
alias  refmsb: std_logic is pwmrefcnt(9);
signal oldrefmsb: std_logic;
signal FIFORead: std_logic; 
signal Start: std_logic; 
signal HostStart: std_logic; 
signal PopData: STD_LOGIC_VECTOR (15 downto 0);
alias PWMPtr: STD_LOGIC_VECTOR (1 downto 0) is PopData(1 downto 0);
alias  Aflag: std_logic is PopData(4);
signal PushData: STD_LOGIC_VECTOR (15 downto 0);
signal FIFOdatawr: STD_LOGIC;
signal IncDataCount: STD_LOGIC;
signal IncDataCountp: STD_LOGIC;
signal PushPtr: STD_LOGIC_VECTOR (7 downto 0);
signal PopPtr: STD_LOGIC_VECTOR (7 downto 0);
signal FPopPtr: STD_LOGIC_VECTOR (7 downto 0);
signal DataCount: STD_LOGIC_VECTOR (8 downto 0);
signal pwma: STD_LOGIC_VECTOR (9 downto 0);
signal pwmb: STD_LOGIC_VECTOR (9 downto 0);
signal pwmc: STD_LOGIC_VECTOR (9 downto 0);
signal pwmouta: STD_LOGIC;
signal pwmoutb: STD_LOGIC;
signal pwmoutc: STD_LOGIC;

component FIFOMem IS
	port (
	addra: IN std_logic_VECTOR(7 downto 0);
	addrb: IN std_logic_VECTOR(7 downto 0);
	clka: IN std_logic;
	clkb: IN std_logic;
	dina: IN std_logic_VECTOR(15 downto 0);
	dinb: IN std_logic_VECTOR(15 downto 0);
	douta: OUT std_logic_VECTOR(15 downto 0);
	doutb: OUT std_logic_VECTOR(15 downto 0);
	wea: IN std_logic;
	web: IN std_logic);
end component FIFOMem;

begin
 AFIFO: FIFOMem port map (
	addra => FPopPtr,
	addrb	=> PushPtr,
	clka => clk,
	clkb => clk,
	dina => x"0000",
	dinb => ibus,
	douta => PopData,
	doutb	=> PushData,
	wea  => '0',
	web  => FIFOdatawr
	);

 	athreephase: process  (clk,addr,readstb,writestb,
									creg, pwmouta, pwmoutb, pwmoutc
			                 ) 
	begin
		if clk'event and clk = '1' then
			IncDataCount <= IncDataCountP;
			
			if (IncDataCount = '1') and (FIFOREAD = '0') then
				IncDataCount <= '0';
				IncDataCountP <= '0';
				DataCount <= DataCount + 1;
			end if;


		   if (UNSIGNED(pwmrefcnt) < UNSIGNED(pwma)) then 
				pwmouta <= '1'; 
			else 
				pwmouta <= '0';
			end if;
		   if (UNSIGNED(pwmrefcnt) < UNSIGNED(pwmb)) then 
				pwmoutb <= '1'; 
			else 
				pwmoutb <= '0';
			end if;
		   if (UNSIGNED(pwmrefcnt) < UNSIGNED(pwmc)) then 
				pwmoutc <= '1'; 
			else 
				pwmoutc <= '0';
			end if;						
		
			oldrefmsb <= refmsb;
											

			if FIFORead = '1' and DataCount /= 0 then
				case PWMPtr is
					when "00" => 
						pwma <= PopData(15 downto 6);						
					when "01"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   => 
						pwmb <= PopData(15 downto 6);							
					when "10" => 
						pwmc <= PopData(15 downto 6);
					when others =>  null;
				end case;				
				PopPtr <= FPopPtr;
				DataCount <= DataCount -1;			
			end if; 			
		
			if writestb = '1' then 
				case addr is
					when "00" => PushPtr <= PushPtr + 1;
									 IncDataCountp <= '1';
					when "01" => creg <= ibus(2 downto 0);
					when "10" => Datacount <= (others => '0');
									 PopPtr <= (others => '0');
									 PushPtr <=	(others => '0');												
					when others => null;
				end case; 						
			end if;				
		
		end if;  -- clk
	
		if ((Start = '1') or (AFlag = '0')) and (DataCount /= 0)  then 
			FIFORead <= '1';
		   FPopPtr <= PopPtr +1;
		else
			FIFORead <= '0';	
			FPopPtr <= PopPtr;	
		end if; 
	
		
		if writestb = '1' and addr = "11" then
			HostStart <= '1';
		else
			HostStart <= '0';
		end if;	

		if writestb = '1' and addr = "00" then
			FIFOdataWr <= '1';
		else
			FIFODataWr <= '0';
		end if;			
			
		if (oldrefmsb = '1' and refmsb = '0' and AFlag = '1' and creg(0) = '1') or (Hoststart = '1' ) then
			Start <= '1';
		else
			Start <= '0';		
		end if; 


	
		obus <= "ZZZZZZZZZZZZZZZZ";
		if readstb = '1' then
			case addr is
				when "00" => obus <= PopData;					
				when "01" => obus <= (2 => creg(2),1 => creg(1),0 =>creg(0), others => '0');
				when "10" => obus(8 downto 0) <= DataCount; 					  								
				when others => obus <= "ZZZZZZZZZZZZZZZZ";	
			end case; 			
		else
			obus <= "ZZZZZZZZZZZZZZZZ";		
		end if;
		if creg(1) = '1' then
			if creg(2) = '0' then
				outa <= pwmouta;
				outb <= pwmoutb;
				outc <= pwmoutc;
			else
				outa <= pwmoutb;
				outb <= pwmouta;
				outc <= pwmoutc;
			end if;	
		else
			outa <= 'Z';
			outb <= 'Z';
			outc <= 'Z';
		end if;		
	end process;

end behavioral;
