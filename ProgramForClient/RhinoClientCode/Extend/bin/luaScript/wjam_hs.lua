
function is_window_exist(title)
    if Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME) > 0 then
        return true;
    else
        return false;
    end
end

function startup_hs()
    print("启动炉石传说");
    -- 判断游戏是否启动，如果没有，等待用户登录战网进入炉石传说
    if is_window_exist(WINDOW_TITLE_HS_GAME) then
        -- 已进入游戏
        return WJAM_RES_OK;
    else
        -- 未进入游戏
        if not is_window_exist(WINDOW_TITLE_HS_BATTLENET) then
            -- 战网未启动，等待用户启动战网
            if not is_window_exist(WINDOW_TITLE_HS_BATTLENET) then
                -- Inf_StartUp()
            end
        else
            -- 战网已启动

        end
    end
end

function start_game()

    local isEnd = false;
    local step = 0;

    while
        not isEnd
    do
        if (0 == step) then
            -- 添加好友
        elseif (1 == step) then
        elseif (2 == step) then
        elseif (3 == step) then
        end
    end
end


function startup_hs()
    print("启动炉石传说")
end



function mouse_click_wait(hWnd, x, y, seconds)
    print("鼠标点击： (", x, ",", y, ")");
    mouseClick(hWnd, x, y, 0);
    Sleep(seconds);
end


KEY_CODE_ENTER = 13 

-- 发送添加好友请求
function reqAddFriend(hWnd, player)
    -- 点击左下角按钮
    mouse_click_wait(hWnd, 0.03, 0.97, 100);
    -- 点击添加按钮
    mouse_click_wait(hWnd, 0.05, 0.90, 100);
    -- 输入用户名
    print("[炉石] 开始游戏, 对手是：", player);
    Inf_Input(hWnd, player)
    -- 按回车键
    Inf_PressKey(hWnd, KEY_CODE_ENTER)

    -- 点击左下角按钮
    mouse_click_wait(hWnd, 0.03, 0.97, 100);
end

-- 接受好友添加请求
function acceptFriendReq(hWnd)
    -- 点击左下角按钮
    mouse_click_wait(hWnd, 0.03, 0.97, 100);
    
    -- 好友邀请确认按钮
    mouse_click_wait(hWnd, 0.157, 0.4454, 100);

end

function reqChallenge(hWnd)
    -- 点击好友管理按钮
    mouse_click_wait(hWnd, 0.03, 0.97, 100);

    -- 点击好友挑战按钮
    mouse_click_wait(hWnd, 0.18, 0.537, 100);

    -- 选择挑战模式
    mouse_click_wait(hWnd, 0.2835, 0.54, 100);
end

function acceptChallenge(hWnd)
    -- 点击接受挑战按钮
    mouse_click_wait(hWnd, 0.55, 0.668, 100);
end




function play_hs(player)
    print("[炉石] 开始游戏, 对手是：", player);

    --    if nil == player then
    --        print("Hello World");
    --        return;

    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME)

    if hWnd > 0 then
        --reqAddFriend(hWnd, "叶落的季节#51207");
        --acceptFriendReq(hWnd);
        reqChallenge(hWnd);
        --acceptChallenge(hWnd);
        --mouse_click_wait(hWnd, 0.5, 0.36, 100);
        -- mouse_click_wait(hWnd, 0.03, 0.93, 100);
        -- mouse_click_wait(hWnd, 0.20, 0.50, 100);
        -- mouse_click_wait(hWnd, 0.30, 0.50, 100);
        -- mouse_click_wait(hWnd, 0.5, 0.295, 1000);	--对战模式
        -- --mouse_click_wait(hWnd, 0.5, 0.375, 1000);  --冒险模式
        -- mouse_click_wait(hWnd, 0.78, 0.78, 1000);
        -- mouse_click_wait(hWnd, 0.78, 0.78, 1000);
        -- mouse_click_wait(hWnd, 0.74, 0.79, 1000);
        -- mouse_click_wait(hWnd, 0.74, 0.79, 1000);
        -- mouse_click_wait(hWnd, 0.8, 0.16, 1000);
        -- mouse_click_wait(hWnd, 0.92, 0.87, 1000);
        -- mouse_click_wait(hWnd, 0.92, 0.87, 1000);
    end
end