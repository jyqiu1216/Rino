print("--LUA--: hs_image 加载开始");

-- distinguish_map={
	-- game_user_menu={"images/hs/user.png"},
	-- game_start={"images/hs/start.png"},
	-- game_main_gui={"images/hs/main_gui.png"},
	-- game_friend={"images/hs/friendGUI.png"},
	-- game_fight={"images/hs/fight.png"},
	-- game_challenge={"images/hs/challenge.png"},
	-- game_win={"images/hs/win.png"},
	-- game_fail={"images/hs/fail.png"},
	-- game_accept_challenge={"images/hs/accept_challenge.png"},
	-- game_task={"images/hs/task.png"}
-- }


-- distinguish_map ={
    -- {name="HS_MENU", file="images/hs/main_gui.png", threshold=125, rect={0.2, 0.2, 0.2, 0.2}},
    -- {name="HS_FRIEND", file="images/hs/friendGUI.png", threshold=50},
    -- {name="HS_ACCEPT_CHALLENGE", file="images/hs/accept_challenge.png", threshold=50},
	-- {name="HS_CHOOSE", file="images/hs/fight.png", threshold=50},
	-- {name="HS_VS", file="images/hs/vs.png", threshold=200},
    -- {name="HS_WIN", file="images/hs/win.png", threshold=100},
	-- {name="HS_FAIL", file="images/hs/fail.png", threshold=100},
	-- {name="HS_TASK", file="images/hs/task.png", threshold=50},
	-- {name="HS_SETTING", file="images/hs/setting.png", threshold=80},
	
	-- {name="HS_END-001", file="images/hs/END-001.png", threshold=200},
	-- {name="HS_END-002", file="images/hs/END-002.png", threshold=200},
	-- {name="HS_END-003", file="images/hs/END-003.png", threshold=220},
	-- {name="HS_END-004", file="images/hs/END-004.png", threshold=220}
-- }  
distinguish_map ={
    {name="HS_MENU", file="images/hs/main_gui.png", threshold=50, rect={0.2, 0.2, 0.2, 0.2}},
    {name="HS_FRIEND", file="images/hs/friendGUI.png", threshold=20},
    {name="HS_ACCEPT_CHALLENGE", file="images/hs/accept_challenge.png", threshold=20},
	{name="HS_CHOOSE", file="images/hs/fight.png", threshold=30},
	{name="HS_VS", file="images/hs/vs.png", threshold=15},
    {name="HS_WIN", file="images/hs/win.png", threshold=30},
	{name="HS_FAIL", file="images/hs/fail.png", threshold=30},
	{name="HS_TASK", file="images/hs/task.png", threshold=20},
	{name="HS_SETTING", file="images/hs/setting.png", threshold=20},
	
	{name="HS_END-001", file="images/hs/END-001.png", threshold=30},
	{name="HS_END-002", file="images/hs/END-002.png", threshold=30},
	{name="HS_END-003", file="images/hs/END-003.png", threshold=30},
	{name="HS_END-004", file="images/hs/END-004.png", threshold=30}
}                                                                                                                

print("--LUA--: hs_image 加载结束");
