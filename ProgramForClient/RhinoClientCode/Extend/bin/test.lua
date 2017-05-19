print("test.lua -begin\n");
EDataType ={
	BYTE_TYPE = 0,
	UBYTE_TYPE = 1,
	SHORT_TYPE = 2,
	USHORT_TYPE = 3,
	INT_TYPE = 4,
	UINT_TYPE = 5,
	FLOAT_TYPE = 6,
	DOUBLE_TYPE = 7,
	INT64_TYPE = 8,
	UINT64_TYPE = 9,
	STRING_TYPE = 10,			--string类型，大小不得超过256个字符
	ADDRESS_TYPE = 11			--返回地址类型
};

-- 系统标准地址，主要偏移类为主，
-- 系统通过lua进行初始化计算，计算完毕后通知后台进行数据信息初始化，
-- 并发送事件通知远程程序获取根据指令获取信息

LOL_GAME_OFFSET_ADD={
--游戏编号基础地址，游戏编号为long数据， 内存信息 OFFSET_游戏编号  [0x154CCB4]+0x00000060  
	GAME_CODE_BASE_OFFSET = 0x154CCB4,
	GAME_CODE_OFFSET_1 = 0x00000060,
-- 用户角色名称地址偏移
	ROLE_OFFSET = 0x154CB20,
--信息板指针
	CENTERHUD_OFFSET = 0x154B9A4,
}

-- 计算完毕之后的地址，系统可以直接进行调用
LOL_GAME_ADD ={
-- 游戏编号
	GAME_CODE_ADD = 0,
-- 用户名
	USER_NAME = LOL_GAME_OFFSET_ADD.ROLE_OFFSET,
-- 英雄名称
	HERO_NAME = 0,
--英雄等级
	HERO_LEVEL = 0,
	 
}


--游戏编号取地址

function LOL_Address_init()
	 
end
-- result = lua_readptr(EDataType.UBYTE_TYPE,0x0049e040);
-- lua_readptr(EDataType.INT_TYPE,result);

print("test.lua -end\n");