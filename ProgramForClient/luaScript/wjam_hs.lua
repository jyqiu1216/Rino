
function is_window_exist(title)
    if Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME) > 0 then
        return true;
    else
        return false;
    end
end


function mouse_click_wait(hWnd, x, y, seconds)
    print("--LUA--:  鼠标点击： (", x, ",", y, ")");
    Inf_MouseClickLButton1(hWnd, x, y, 1)
    Sleep(seconds);
end


KEY_CODE_ENTER = 13 

function startup_hs()
    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME);
    local count = 0;

    while
        (hWnd <= 0)
    do
        print("--LUA--:  游戏窗口不存在", count);
        Sleep(1000)
        count = count + 1;
        hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME);
    end

    local tick = 3;
    print("--LUA--:  获得游戏窗口句柄,", tick, "秒后开始注入DLL ==>  ", WINDOW_TITLE_HS_GAME, EXTEND_DLL_HS);
    while
        tick > 0
    do
        print("--LUA--:  等待......", tick, "秒");
        Sleep(1000);
        tick = tick - 1;
    end

    print("--LUA--:  开始执行注入:BEGIN");
    Lua_InjectDll(WINDOW_TITLE_HS_GAME, EXTEND_DLL_HS);

    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
    --mouse_click_wait(hWnd, 0.5, 0.2, 100);
    print("--LUA--:  开始执行注入:END");
end


-- 发送添加好友请求
function reqAddFriend(hWnd, player)
    
    print("--LUA--:  添加好友：", player);

    -- 激活游戏窗口
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
     Sleep(500);

    -- 点击左下角按钮
    -- mouse_click_wait(hWnd, 0.03, 0.98, 100);

    -- 点击添加按钮
    local x, y, w, h = Inf_GetHS_FriendWindowRect();

   
    mouse_click_wait(hWnd, x + w * 0.24, y + h * 0.935, 100);

    -- 输入用户名

    Inf_Input(hWnd, player)

    -- 按回车键
    Inf_PressKey(hWnd, KEY_CODE_ENTER)
    Sleep(100);

    -- 点击空白地方按钮
    --mouse_click_wait(hWnd, 0.5, 0.95, 1000);
end

-- 接受好友添加请求
function acceptFriendReq(hWnd)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);

    -- 点击左下角按钮
    -- mouse_click_wait(hWnd, 0.03, 0.97, 100);

    local x, y, w, h = Inf_GetHS_FriendWindowRect();

    -- 好友邀请确认按钮
    mouse_click_wait(hWnd, x + w * 0.7, y + h * 0.23, 100);
	Sleep(1000);
	
	print("================");
    -- 点击空白地方
    mouse_click_wait(hWnd, 0.5, 0.95, 100);
	Sleep(1000);
end

function reqChallenge(hWnd, x, y)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
    Sleep(1000);
   
    -- 点击挑战模式选择按钮
	mouse_click_wait(hWnd, x, y, 1000);

    -- 点击挑战模式选择按钮
	mouse_click_wait(hWnd, x + 120, y, 1000);

    -- 点击空白地方
    mouse_click_wait(hWnd, 0.5, 0.95, 1000);
end

function acceptChallenge(hWnd)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
    Sleep(1000);
   
    -- 点击接受挑战按钮
    mouse_click_wait(hWnd, 0.45, 0.62, 100);

    -- 点击空白地方
    mouse_click_wait(hWnd, 0.5, 0.95, 1000);
end
