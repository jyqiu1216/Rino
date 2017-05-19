lua_import("luaScript/define.out")
lua_import("luaScript/checkWindowPixel.out")
lua_import("luaScript/mouse_event.out")
lua_import("luaScript/wjam_hs.out")


function login_lol(serverid)
    print("--LUA--:  ====================BEGIN:", "登陆LOL")

    local loopflag = true
    local step = 1
    local loopCount = 0

    local hWnd = 0

    while
        loopflag
    do


        print("--LUA--:  =====> step:", step)
        if (1 == step) then
            hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_LOGIN)

            if hWnd <= 0 then
                print("--LUA--:  LOL登陆窗口不存在!", 1000 * loopCount)
                Sleep(1000)

                if loopCount > 90 then
                    print("--LUA--:  启动登陆窗口超时", loopCount, "秒")
                    return WJAM_RES_TIMEOUT_LOL_LOGIN_STARTUP
                end
                loopCount = loopCount + 1
            else
                step = 2
                loopCount = 0
            end

        elseif (2 == step) then
            hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_LOGIN)

            if hWnd <= 0 then
                print("--LUA--:  窗口不存在", WINDOW_TITLE_LOL_LOGIN)
                return WJAM_RES_WINDOW_NOT_EXIST
            end

            -- 窗口存在
            if loopCount == 0 then
                Inf_ActiveWindow(WINDOW_TITLE_LOL_LOGIN)
                Sleep(500)
                loopCount = loopCount + 1
            end

            if Lua_GetWindowPixelRGB(WINDOW_TITLE_LOL_LOGIN, pixel_login_page2) > 0 then
                print("--LUA--:  登陆页面：选择游戏服务器")
                step = 3
                loopCount = 0
            elseif Lua_GetWindowPixelRGB(WINDOW_TITLE_LOL_LOGIN, pixel_login) > 0 then
                print("--LUA--:  登陆页面：等待用户登录")
                Sleep(100)
            else
                Inf_ActiveWindow(WINDOW_TITLE_LOL_LOGIN)
                --Sleep(100)
                print("--LUA--:  ==> 登陆页面：等待加载......")
                --loopCount = loopCount + 1
                --if loopCount*100 > 90000 then
                --    print("--LUA--:  LOL登陆窗口加载超时")
                --    return WJAM_RES_TIMEOUT_LOL_LOGIN_LOADING
                --end
            end

        elseif (3 == step) then
            -- 选择服务器
            hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_LOGIN)

            if hWnd <= 0 then
                return WJAM_RES_WINDOW_NOT_EXIST
                -- 窗口异常
            else
                Inf_ActiveWindow(WINDOW_TITLE_LOL_LOGIN)
                -- Sleep(1000)

                if (Lua_GetWindowPixelRGB(WINDOW_TITLE_LOL_LOGIN, pixel_serverlist) > 0) then
				
					--lua_TestDebug("xxxxx");
                    print("--LUA--:  进入选择服务器页面", serverid, server[serverid][2][1], server[serverid][2][2])
					Sleep(2000)
					mouseClick(hWnd, servershow[1], servershow[2], 0)
					Sleep(2000)
                    mouseClick(hWnd, server[serverid][2][1], server[serverid][2][2], 0)
					Sleep(2000)
                    mouseClick(hWnd, enterserver[1], enterserver[2], 0)
                    step = 4
                    loopCount = 0
                else

                    if (loopCount > 30) then
                        return WJAM_RES_TIMEOUT_LOL_SELECT_SERVER
                    end
					
                    print("--LUA--:  服务器列表未显示：点击服务器列表按钮")
                    mouseClick(hWnd, servershow[1], servershow[2], 0)
                    loopCount = loopCount + 1
                    Sleep(300)
                end
            end

        elseif (4 == step) then
            hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_CLIENT)
            if hWnd <= 0 then
                print("--LUA--:  游戏大厅窗口不存在，等待加载...")
                Sleep(1000)
                loopCount = loopCount + 1
                if (loopCount > 150) then
                    return WJAM_RES_TIMEOUT_LOL_HALL_STARTUP
                end
            else
                print("--LUA--:  检测是否进入游戏大厅")
                Sleep(1000)

                Lua_SetWindowVisibleByClassName(WINDOW_CLASS_TXGuiFoundation, 0)
                if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_hall) > 0 then
                    print("--LUA--:  进入游戏大厅")
                    Sleep(3000)
                    -- Lua_SetWindowVisibleByTitle(WINDOW_TITLE_LOL_CLIENT, 0)
                    Lua_SetWindowVisibleByClassName(WINDOW_CLASS_TXGuiFoundation, 0)
                    -- Lua_SetWindowVisibleByClassName();

                    Inf_SetWindowVisible(hWnd, 0)

                    loopCount = 0
                    loopflag = false
                elseif Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_reconn) > 0 then
                    print("--LUA--:  断线重连")
                    return WJAM_RES_LOL_LOGIN_FROM_DISCONNECTION
                    --    -- 用户处于断线重连状态
                    -- elseif Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_server_overload)--服務器滿載
                else
                    print("--LUA--:  等待游戏大厅加载...", loopCount)
                    loopCount = loopCount + 1
                    Sleep(1000)
                    if (loopCount > 150) then
                        return WJAM_RES_TIMEOUT_LOL_HALL_LOADING
                    end
                end
            end
        end
    end
    print("--LUA--:  ====================END:", "登陆LOL")
    return WJAM_RES_OK
end

-- 登录入口
function login(gameid, serverid, ptah)
    print("--LUA--:  gameid=", gameid, GAME_LOL)
    if gameid == GAME_LOL then
        print("--LUA--:  =====> LOL")
        local res = login_lol(serverid)
        print(res)
        return res
    elseif gameid == GAME_HB then
        print("--LUA--:  =====> Hearth Stone")
        return 0
        -- loginhb(account, password)
    end
end


-- 是否是房间页面
function checkroom()
    local tb = { }
    INF_getWindowPixelRGB(pic_room, tb, WINDOW_TITLE_LOL_CLIENT)
    while
        (not compareposvalue(room_value, tb))
    do
        Sleep(500)
        INF_getWindowPixelRGB(pic_room, tb, WINDOW_TITLE_LOL_CLIENT)
    end
end



function is_roomname_exist()
    compareWindowPixel()
end



-- 创建房间入口
function createroom(roomname, password, account, mode, map, playernum, observe, gametype)
    print("--LUA--:  =====> 开始创建房间")
    Lua_SetWindowVisibleByClassName(WINDOW_CLASS_TXGuiFoundation, 0)

    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_CLIENT)
    print(hWnd)
    if (hWnd <= 0) then
        return WJAM_RES_WINDOW_NOT_EXIST
    end

    Inf_SetWindowVisible(hWnd, 0)

    local step = 1
    local loopFlag = true
    local loopCount = 0

    while
        loopFlag
    do

        if 1 == step then
            if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_hall) <= 0 then
                print("--LUA--:  =====> 不在游戏大厅，点击左上角按钮返回大厅")
                mouseClick(hWnd, hallpos[1], hallpos[2], 1)
                Sleep(1000)

                loopCount = loopCount + 1
                if loopCount > 60 then
                    print("--LUA--:  返回游戏大厅超时")
                    return WJAM_RES_TIMEOUT
                end
            else
                print("--LUA--:  点击PLAY按钮")
                -- 点击play按钮
                mouseClick(hWnd, playbutton[1], playbutton[2], 0)
                step = 2
                loopCount = 0
            end

        elseif 2 == step then
            -- 点击自定义游戏
            mouseClick(hWnd, customgame[1], customgame[2], 0)

            -- 点击创建游戏
            mouseClick(hWnd, creategame[1], creategame[2], 0)
		    Sleep(2000)
			-- 选择地图
			print("选择地图:", map)
            mouseClick(hWnd, mapTypePos[map][1], mapTypePos[map][2], 0)
			Sleep(200)
			
            -- 输入房间名
            mouseThreeClick(hWnd, roomnamepos[1], roomnamepos[2], 0)
            Inf_Backspace(hWnd)
            Inf_Input(hWnd, roomname)
            Sleep(200)

            -- 输入密码
            mouseDoubleClick(hWnd, roompasswordpos[1], roompasswordpos[2], 0)
            Inf_Input(hWnd, password)
            Sleep(200)

            -- 选择队伍人数
            print("--LUA--:  选择队伍人数:", playernum)
            mouseClick(hWnd, playerNumPos[1][1], playerNumPos[1][2], 0)
            Sleep(300)
            mouseClick(hWnd, playerNumPos[playernum + 1][1], playerNumPos[playernum + 1][2], 0)


            -- 选择游戏类型
			print("--LUA--:  选择游戏类型:", gametype)
            mouseClick(hWnd, gameTypePos[1][1], gameTypePos[1][2], 0)
            Sleep(300)
            mouseClick(hWnd, gameTypePos[gametype + 1][1], gameTypePos[gametype + 1][2], 0)

            -- 点击创建房间
			print("--LUA--:  点击创建房间按钮")
            mouseClick(hWnd, createRoomBtnPos[1], createRoomBtnPos[2], 0)
            Sleep(2000)
            

            if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_room_repait) > 0 then
                mouseClick(roomrepaitok[1], roomrepaitok[2], 0)
                return WJAM_RES_ROOM_EXIST
            end

            step = 3
        elseif 3 == step then
            loopFlag = false
            -- if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_room) > 0 then
            --    loopFlag = false
            -- else
            --    print("--LUA--:  检查是否进入房间")
            -- end
        end
    end

    print("--LUA--:  创建房间结束")
    --Inf_SetWindowVisible(hWnd, 0);
    return WJAM_RES_OK
end


-- 查找加入按钮位置
function findjoinbutton(side)
    local pos = { }
    local tb = { }
    local pic_pos = { }
    local i = 1
    if side == 1 then
        pic_pos = pic_redjoin
    else
        pic_pos = pic_bulejoin
    end
    INF_getWindowPixelRGB(pic_pos, tb, WINDOW_TITLE_LOL_CLIENT)
    while
        (i <= 5)
    do
        -- print(tb[i][1],':',tb[i][2],':',tb[i][3])
        -- print(redjoin_value[1],':',redjoin_value[2],':',redjoin_value[3])
        if tb[i][1] == redjoin_value[1] and tb[i][2] == redjoin_value[2] and tb[i][3] == redjoin_value[3] then
            print("--LUA--:  找到位置");
            return pic_pos[i]
        end
        i = i + 1
    end
end

function is_page_hall_main()
    return compareWindowPixel(pic_hall, hall_value, WINDOW_TITLE_LOL_CLIENT)
end



-- 加入房间入口
function joinroom(roomname, password, account, side)
    print("--LUA--:  ==========>加入房間<==========", roomname)

    -- Lua_SetWindowVisibleByTitle(WINDOW_TITLE_LOL_CLIENT, 1)
    Lua_SetWindowVisibleByClassName(WINDOW_CLASS_TXGuiFoundation, 0)


    local exitFlag = false

    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_CLIENT)
    if (hWnd <= 0) then
        print("--LUA--:  LOL游戲客戶端窗口不存在")
        return WJAM_RES_WINDOW_NOT_EXIST
    end

    Inf_SetWindowVisible(hWnd, 1)

    local step = 1
    local loopFlag = true
    local loopCount = 0
    while
        loopFlag
    do
        -- Inf_ActiveWindow(WINDOW_TITLE_LOL_CLIENT)

        if 1 == step then
            if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_hall) <= 0 then
                print("--LUA--:  不在游戏大厅或游戏大厅PLAY按钮不可用，点击左上角按钮返回大厅")
                mouseClick(hWnd, hallpos[1], hallpos[2], 1)
                Sleep(200)

                loopCount = loopCount + 1
                if loopCount > 300 then
                    print("--LUA--:  游戏大厅PLAY按钮不可用")
                    return WJAM_RES_TIMEOUT_LOL_HALL_PLAY_DISABLE
                end
            else
                print("--LUA--:  点击PLAY按钮")
                -- 点击play按钮
                mouseClick(hWnd, playbutton[1], playbutton[2], 0)
                step = 2
                loopCount = 0
            end

        elseif 2 == step then

            mouseClick(hWnd, customgame[1], customgame[2], 0)
            -- 点击自定义游戏按钮
            mouseClick(hWnd, joingame[1], joingame[2], 0)
            -- 点击加入游戏按钮
            Sleep(1000)

            mouseClick(hWnd, showpasswd[1], showpasswd[2], 0)
            -- 显示密码
            Sleep(1000)

            -- 房间搜索
            print("--LUA--:  -----------------------------")
            mouseThreeClick(hWnd, searchroom[1], searchroom[2], 0)
            Inf_Backspace(hWnd)
            Inf_Input(hWnd, roomname)
            Sleep(300)

            step = 3
        elseif 3 == step then
            if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_room_exist) > 0 then
                print("--LUA--:  房间存在")

                -- 进入房间
                mouseDoubleClick(hWnd, roompos[1], roompos[2], 0)
                Inf_Input(hWnd, password)
                Sleep(300)

                mouseClick(hWnd, confirmpasswd[1], confirmpasswd[2], 1)
                -- 点击密码确定按钮

                loopFlag = false
            else
                print("--LUA--:  游戏房间不存在")
                --mouseClick(hWnd, pos_refresh_roomlist[1], pos_refresh_roomlist[2], 0);
                return WJAM_RES_LOL_ROOM_NOT_EXIST
            end

            -- checkroom()

            -- 选择阵营
            -- print("--LUA--:  -----")
            -- local joinbutton = { }
            -- joinbutton = findjoinbutton(side)
            -- print("--LUA--:  -----")
            -- if joinbutton ~= nil then
            -- print("--LUA--:  -----")
            -- print(joinbutton[1], ":", joinbutton[2])
            -- mouseClick(joinbutton[1], joinbutton[2], 0)
            -- end
        end
    end
	return WJAM_RES_OK
end



function playgame(gameid, isGameOwner)
    print("--LUA--:  ==========>开始游戏<==========")

    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_LOL_CLIENT)
    if hWnd <= 0 then
        return WJAM_RES_WINDOW_NOT_EXIST
    end

    Sleep(1000)

    if isGameOwner then
        if Lua_ComparePixel(WINDOW_TITLE_LOL_CLIENT, pixel_room) then
            print("--LUA--:  =====> 进入游戏大厅")
        else
            print("--LUA--:  not")
        end

        print(pos_start[1], pos_start[2])
        mouseClick(hWnd, pos_start[1], pos_start[2], 0)
        Sleep(200)
        mouseClick(hWnd, 590, 485, 0)
    end

    Inf_SetWindowVisible(hWnd, 1)

    while
        INF_IsWindowExist(WINDOW_TITLE_LOL_GAME) <= 0
    do
        print("--LUA--:  游戏窗口未启动，等待加载...")
        Sleep(5000)
    end

    print("--LUA--:  10秒后开始注入")
    Sleep(10000)
    print("--LUA--:  注入DLL", WINDOW_TITLE_LOL_GAME, INJECTED_DLLNAME_LOL)
    Lua_InjectDll(WINDOW_TITLE_LOL_GAME, INJECTED_DLLNAME_LOL);

    return WJAM_RES_OK
end


function injectDll(gameid)
    print("--LUA--:  ==========>注入DLL<==========", gameid)
    if GAME_LOL == gameid then
        Lua_InjectDll(WINDOW_TITLE_LOL_GAME, INJECTED_DLLNAME_LOL)
    elseif GAME_HB == gameid then
    end
end



-- 加载lua入口
function main()
    print("--LUA--:  加载 main.lua 成功.")
end

main()
