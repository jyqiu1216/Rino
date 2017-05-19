
base_internal_path = {}

-- 各个基址
base_internal_path["GameCodeBase"] = "0x5E3C60"	--游戏编号
base_internal_path["KillTowerBase1"] = "0x1A1CBF0"	--房主推塔地址
base_internal_path["KillTowerBase2"] = "0x1A2309C"	--非房主推塔地址
base_internal_path["GameResultBase"] = "0x1A226A4" --游戏结果
base_internal_path["TimeStartBase"] = "0x36A3AA0" --时间对象
base_internal_path["PlayBase"] = "0x1A1D794" --玩家角色名称
base_internal_path["CenterHudBase"] = "0x1A22EC8" --角色信息
base_internal_path["g_ExitGameCall"] = "0xA9E140" --退出游戏



--【1】游戏编号
function FunGetValue_1()
	local res = GetGameKey(base_internal_path["GameCodeBase"], 0x70)
return res
end

--【2】推塔数（房主）
function FunGetValue_2()
	local res = base_internal_path["KillTowerBase1"]
return res
end

--【3】推塔数（非房主）
function FunGetValue_3()
	local res = base_internal_path["KillTowerBase2"]
return res
end

--【4】游戏结果
function FunGetValue_4()
	local res = base_internal_path["GameResultBase"]
return res
end

--【5】开始时间
function FunGetValue_5()
	local res = fun_offset(base_internal_path["TimeStartBase"], 0x18)
return res
end

--【6】玩家角色名称
function FunGetValue_6()
	local res = base_internal_path["PlayBase"]
return res
end

--【7】击杀数
function FunGetValue_7()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0x24)
return res
end

--【8】死亡数
function FunGetValue_8()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0x28)
return res
end

--【9】助攻数
function FunGetValue_9()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0x2c)
return res
end

--【10】补刀数
function FunGetValue_10()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0x24)
return res
end

--【11】退出游戏
function FunGetValue_11()
	return base_internal_path["g_ExitGameCall"];
end