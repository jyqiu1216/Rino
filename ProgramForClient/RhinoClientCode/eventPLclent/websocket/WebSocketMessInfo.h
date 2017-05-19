#ifndef WJ_WebSocketMessInfo_h
#define WJ_WebSocketMessInfo_h

const static int WEB_SOCKET_PORT = 23100;//端口号c++与H5通信
const static int WEB_SOCKET_PORT_H5_TO_HS = 12351;//端口号H5与炉石注入的通信

//c++与H5交付处理
const static int WEB_SOCKET_1000 = 1000;//连接成功(c++ -> H5)

const static int WEB_SOCKET_1001 = 1001;//玩家登陆信息交付(H5 -> c++)
const static int WEB_SOCKET_1002 = 1002;//获取非任务赛的所有赛事列表数据交付(H5 -> c++)
const static int WEB_SOCKET_1003 = 1003;//游戏区服列表交付(H5 -> c++)
const static int WEB_SOCKET_1004 = 1004;//任务赛大厅交付(H5 -> c++)
const static int WEB_SOCKET_1005 = 1005;//任务赛报名交付		!1005 1006顺序倒了，有时间再改(H5 -> c++)
const static int WEB_SOCKET_1006 = 1006;//任务赛详情页面交付(H5 -> c++)
const static int WEB_SOCKET_1007 = 1007;//任务赛结束页面交付(c++ -> H5)
const static int WEB_SOCKET_1008 = 1008;//solo赛一键开赛点击交付(H5 -> c++)
const static int WEB_SOCKET_1009 = 1009;//团队赛一键开赛点击交付(H5 -> c++)
const static int WEB_SOCKET_1010 = 1010;//获得游戏路径交付(H5 -> c++)
const static int WEB_SOCKET_1011 = 1011;//设置游戏路径交付(H5 -> c++)
const static int WEB_SOCKET_1012 = 1012;//游戏路径错误交付(c++ -> H5)
const static int WEB_SOCKET_1013 = 1013;//点击比赛节点获取房间列表交付(H5 -> c++)
const static int WEB_SOCKET_1014 = 1014;//离开比赛详情页面交付(H5 -> c++)
const static int WEB_SOCKET_1015 = 1015;//封锁比赛详情页面比赛按钮交付(c++ -> H5)
const static int WEB_SOCKET_1016 = 1016;//解锁比赛详情页面比赛按钮交付(c++ -> H5)


const static int WEB_SOCKET_1021 = 1021;//solo赛游戏结束交付(刷新用户数据)(c++ -> H5)
const static int WEB_SOCKET_1027 = 1027;//告知当前是什么游戏id(H5 -> c++)
const static int WEB_SOCKET_1028 = 1028;//告知当前客户端版本(H5 -> c++)
const static int WEB_SOCKET_1029 = 1029;//关闭注册窗口(H5 -> c++)
const static int WEB_SOCKET_1030 = 1030;//游戏结束页面关闭(H5 -> c++)
const static int WEB_SOCKET_1031 = 1031;//请求玩家是否处于游戏状态标志
const static int WEB_SOCKET_1032 = 1032;//游戏结束页面—再来一局(c++ -> H5)
const static int WEB_SOCKET_1034 = 1034;//联赛请求开始(H5 -> c++)


const static int WEB_SOCKET_3001 = 3001;//任务赛一键开赛点击交付(H5 -> c++)
const static int WEB_SOCKET_3002 = 3002;//托盘右键操作呼出个人中心(C++ -> H5)
const static int WEB_SOCKET_3003 = 3003;//托盘右键操作呼出设置页面(C++ -> H5)



const static int WEB_SOCKET_6000 = 6000;//H5取服务器IP
const static int WEB_SOCKET_6001 = 6001;//刷新在线人数(C++ -> H5)

const static int WEB_SOCKET_7000 = 7000;//H5索要注册需要的推广id(H5 -> c++)
const static int WEB_SOCKET_7001 = 7001;//将推广id告知H5(C++ -> H5)

const static int WEB_SOCKET_8001 = 8001;//LOL押注页数据传送
const static int WEB_SOCKET_8002 = 8002;//LOL押注通知大厅切换到兑换奖品
const static int WEB_SOCKET_8008 = 8008;//LOL押注弹出页关闭
const static int WEB_SOCKET_8009 = 8009;//LOL押注页兑换奖品

const static int WEB_SOCKET_9001 = 9001;//微信登陆二维码扫描窗口打开交付(H5 -> c++)
const static int WEB_SOCKET_9002 = 9002;//微信登陆二维码扫描窗口关闭交付(H5 -> c++)
const static int WEB_SOCKET_9003 = 9003;//微信登陆二维码扫描拿到用户信息进行转发交付(H5 -> c++)
const static int WEB_SOCKET_9004 = 9004;//微信登陆二维码扫描通知大厅使用账户登陆交付(H5 -> c++)
const static int WEB_SOCKET_9005 = 9005;//微信登陆二维码扫描通知大厅使用key+userid登陆业务服务器交付(H5 -> c++)

const static int WEB_SOCKET_9006 = 9006;//设置中修改密码
const static int WEB_SOCKET_9007 = 9007;//LOL押注H5初始化通讯
const static int WEB_SOCKET_9008 = 9008;//LOL押注H5传回押注信息

const static int WEB_SOCKET_9009 = 9009;//微信注册需要传递推广标志
const static int WEB_SOCKET_9010 = 9010;//玩家是否查看对阵图（H5-->C++）
const static int WEB_SOCKET_9011 = 9011;//普通登录转微信登录(H5 -> c++)
const static int WEB_SOCKET_9012 = 9012;//普通登录窗口关闭交付(H5 -> c++)
const static int WEB_SOCKET_9013 = 9013;//注册窗口打开(H5 -> c++)
const static int WEB_SOCKET_9014 = 9014;//联赛房间内玩家准备消息 
const static int WEB_SOCKET_9015 = 9015;//通知H5刷新对阵图
const static int WEB_SOCKET_9016 = 9016;//炉石联赛玩家pick的英雄（H5-->C++）
const static int WEB_SOCKET_9017 = 9017;//联赛房间内玩家发送聊天消息(H5-->C++)
const static int WEB_SOCKET_9018 = 9018;//联赛房间内玩家收到聊天消息(C++-->H5)
const static int WEB_SOCKET_9019 = 9019;//重新加载大厅页面
const static int WEB_SOCKET_9020 = 9020;//发送PICK消息给H5
const static int WEB_SOCKET_9021 = 9021;//炉石联赛玩家ban的英雄（H5-->C++）


//游戏单独使用的id
///////英雄联盟///////////
const static int WEB_SOCKET_3004 = 3004;//英雄联盟solo赛取消匹配（退费）(H5 -> c++)
const static int WEB_SOCKET_3005 = 3005;//通知H5显示匹配页面(C++ -> H5)
const static int WEB_SOCKET_3006 = 3006;//solo赛区服信息(H5 -> c++)
const static int WEB_SOCKET_3007 = 3007;//solo赛匹配到的玩家信息(H5 -> c++)
const static int WEB_SOCKET_3008 = 3008;//solo赛结束页面信息(H5 -> c++)
const static int WEB_SOCKET_3009 = 3009;//solo匹配成功(C++ -> H5)
const static int WEB_SOCKET_3010 = 3010;//solo赛结果页关闭按钮(H5 -> c++)
const static int WEB_SOCKET_3011 = 3011;//solo赛断线重连游戏ID比赛ID房间ID(C++ -> H5)
const static int WEB_SOCKET_3012 = 3012;//solo赛断线重连成功进入房间ID(H5 -> c++)
const static int WEB_SOCKET_3013 = 3013;//solo赛匹配中LOL异常关闭（需要关闭匹配页面并退费）(C++ -> H5)
const static int WEB_SOCKET_3014 = 3014;//LOL任务赛结果页面关闭


///////炉石传说///////////
const static int WEB_SOCKET_1017 = 1017;//炉石solo赛结果页面交付(H5 -> c++)
const static int WEB_SOCKET_1018 = 1018;//炉石solo赛匹配交付_玩家点击h5中的匹配按钮(H5 -> c++)
const static int WEB_SOCKET_1020 = 1020;//炉石solo赛匹配结果交付(c++ -> H5)
const static int WEB_SOCKET_1022 = 1022; //炉石异常交付(带对手名称)(c++ -> H5)
const static int WEB_SOCKET_1024 = 1024; //炉石solo赛人工干预超时通知(c++ -> H5)
const static int WEB_SOCKET_1025 = 1025;//炉石右上角卡牌状按钮点击交付（1展开，0收起）(c++ -> H5)
const static int WEB_SOCKET_1026 = 1026;//炉石内嵌窗口匹配按钮控制使能交付（1使能，0去使能）(c++ -> H5)
const static int WEB_SOCKET_1033 = 1033;//炉石solo赛复制对手名称(c++ -> H5)
const static int WEB_SOCKET_1035 = 1035;//炉石联赛交付_玩家点击h5中的准备按钮(H5 -> c++)
const static int WEB_SOCKET_1036 = 1036;//炉石联赛结果交付(H5 -> c++)
const static int WEB_SOCKET_1037 = 1037;//炉石联赛_关闭内嵌窗口(H5 -> c++)
const static int WEB_SOCKET_1038 = 1038;//炉石联赛收到对手游戏内已经准备消息(H5 -> c++)
const static int WEB_SOCKET_1039 = 1039;//炉石联赛玩家未在规定时间点击准备按钮(c++ -> H5)
const static int WEB_SOCKET_1040 = 1040;//炉石联赛两人都未准备点击准备(c++ -> H5)







//c++与自动化逆向处理

#endif//WJ_WebSocketMessInfo_h