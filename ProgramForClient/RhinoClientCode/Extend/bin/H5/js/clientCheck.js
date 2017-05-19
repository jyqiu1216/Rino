/**
 * Created by Administrator on 2016/1/11.
 */
/*
 *用户代理检测脚本，检测范围包括浏览器引擎、平台、Windows、移动设备和游戏系统
 */
var client = function()
{
    //呈现引擎
    var engine = {
        ie:0,           //ie
        gecko:0,    //火狐
        webkit:0,   //chrome和safari
        khtml:0,     //Linux下的Konqueror
        opera:0,     //opera
        ver:null      //完整版本号
    };

    //浏览器
    var browser = {
        ie:0,
        firefox:0,
        safari:0,
        konq:0,
        opera:0,
        chrome:0,
        ver:null        //具体版本号
    };

    //平台、设备和操作系统
    var system = {
        //操作系统
        win:false,
        mac:false,
        xll:false,       //UNIX

        //移动设备
        iphone:false,
        ipod:false,
        ipad:false,
        ios:false,
        android:false,
        nokiaN:false,      //诺基亚
        winMobile:false,  //windows mobile

        //游戏系统
        wii:false,          //Wii
        ps:false            //Playstation 3
    };

    //检测呈现引擎和浏览器
    var ua = navigator.userAgent;
    if(window.opera)
    {
        engine.ver = browser.ver = window.opera.version();
        engine.opera = browser.opera = parseFloat(engine.ver);
    }
    else if(/AppleWebkit\/(\S+)/.test(ua ))
    {
        engine.ver = RegExp["$1"];
        engine.webkit = parseFloat(engine.ver);

        //确定是chrome还是safari
        if(/Chrome\/(\S+)/.test(ua))
        {
            browser.ver = RegExp["$1"];
            browser.chrome = parseFloat(browser.ver);
        }
        else if(/Version\/(\S+)/.test(ua))
        {
            browser.ver = RegExp["$1"];
            browser.safari = parseFloat(browser.ver);
        }
        else
        {
            // 近似确定版本号
            var safariVersion = 1;
            if(engine.webkit < 100)
            {
                safariVersion = 1;
            }
            else if(engine.webkit < 312)
            {
                safariVersion = 1.2;
            }
            else if(engine.webkit < 412)
            {
                safariVersion = 1.3;
            }
            else
            {
                safariVersion = 2;
            }
            browser.safari = browser.ver = safariVersion;
        }
    }
    else if(/KHTML\/(\S+)/.test(ua) || /Konqueror\/([^; ]+)/.test(ua))
    {
        engine.ver = browser.ver = RegExp["$1"];
        engine.khtml = browser.konq = parseFloat(engine.ver);
    }
    else if (/rv:([^\)]+)\) Gecko\/\d{8}/.test(ua))
    {
        engine.ver = RegExp["$1"];
        engine.gecko = parseFloat(engine.ver);

        // 确定是不是FireFox
        if(/firefox\/(\S+)/.test(ua))
        {
            browser.ver = RegExp["$1"];
            browser.firefox = parseFloat(engine.ver);
        }
    }
    else if(/MISE ([^; ]+)/.test(ua))
    {
        engine.ver = browser.ver = RegExp["$1"];
        engine.ie = browser.ie = parseFloat(engine.ver);
    }

    // 检测浏览器
    browser.ie = engine.ie;
    browser.opera = engine.opera;

    // 检测平台
    var p = navigator.platform;
    system.win = p.indexOf("Win") == 0;
    system.mac = p.indexOf("Mac") == 0;
    system.xll = (p == "xll") || (p.indexOf("Linux") == 0);

    // 检测windows操作系统
    if(system.win)
    {
        if(/Win(?:dows )?([^do]{2})\s?(\d+\.\d+)?/.test(ua))
        {
            if(RegExp["$1"] == "NT")
            {
                switch(RegExp["$2"])
                {
                    case "5.0":
                        system.win = "2000";
                        break;
                    case "5.1":
                        system.win = "XP";
                        break;
                    case "6.0":
                        system.win = "Vista";
                        break;
                    case "6.1":
                        system.win = "7";
                        break;
                    default:
                        system.win = "NT";
                        break;
                }
            }
            else if(RegExp["$1"] == "9x")
            {
                system.win = "ME";
            }
            else
            {
                system.win = RegExp["$1"];
            }
        }
    }

    // 移动设备
    system.iphone = ua.indexOf("iPhone") > -1;
    system.ipod = ua.indexOf("iPod") > -1;
    system.ipad = ua.indexOf("iPad") > -1;
    system.nokiaN = ua.indexOf("NokiaN") > -1;

    // windows mobile
    if(system.win == "CE")
    {
        system.winMobile = system.win;
    }
    else if(system.win == "Ph")
    {
        if(/Windows Phone OS (\d+.\d+)/.test(ua))
        {
            system.win = "Phone";
            system.winMobile = parseFloat(RegExp["$1"]);
        }
    }

    // 检测IOS版本
    if(system.mac && ua.indexOf("Mobile") > -1)
    {
        if(/CPU (?:iPhone )?OS (\d+_\d+)/.test(ua))
        {
            system.ios = parseFloat(RegExp.$1.replace("_","."));
        }
        else
        {
            system.ios = 2;
        }
    }

    // 检测Android
    if(/Android (\d+\.\d+)/.test(ua))
    {
        system.android = parseFloat(RegExp.$1);
    }

    // 游戏系统
    system.wii = ua.indexOf("Wii") > -1;
    system.ps = /playstation/i.test(ua);

    // 返回结果
    return {
        engine:engine,
        browser:browser,
        system:system
    };
}();
//添加监听事件，移除事监听件，实现浏览器的兼容
var EventUtil = {
    //事件监听
    addHandler : function(target, type, fun){
        if (target.addEventListener) {
            target.addEventListener(type, fun, false);
        }else if (target.attachEvent) {
            target.attachEvent("on" + type, fun);
        }else {
            target["on" + type] = fun;
        };
    },
    //鼠标按钮
    getButton : function(event){
        if (document.implementation.hasFeature("MouseEvents", "2.0")) {
            return event.button;
        }else {
            switch(event.button){
                case 0:
                case 1:
                case 3:
                case 5:
                case 7:
                    return 0;
                case 2:
                case 6:
                    return 2;
                case 4:
                    return 1;
            }
        };
    },
    //字符编码
    getCharCode : function(event){
        if (typeof event.charCode == "number") {
            return event.charCode;
        }else {
            return event.keyCode;
        }
    },
    //获取剪切板文本
    getClipboardText: function(event){
        var clipboardData = (event.clipboardData || window.clipboardData);
        return clipboardData.getData("text");
    },
    //得到事件
    getEvent : function(event){
        return event ? event : window.event;
    },
    //获取相关元素
    getRelatedTarget : function(event){
        if (event.relatedTarget) {
            return event.relatedTarget;
        }else if (event.toElement) {
            return event.toElement;
        }else if (event.fromElement) {
            return event.fromElement;
        }else {
            return null;
        };
    },
    //获取实际目标
    getTarget : function(event){
        return event.target || event.srcElement;
    },
    //鼠标滚轮事件
    getWheelDelta : function(event){
        if (event.wheelDelta) {
            return (client.engine.opera && client.engine.opera < 9.5 ? -event.wheelDelta : event.wheelDelta);
        }else{
            return -event.detail * 40;
        };
    },
    //阻止默认事件
    preventDefault : function(event){
        if (event.preventDefault) {
            event.preventDefault();
        }else {
            event.returnValue = true;
        };
    },
    //移除事件监听
    removeHandler : function(target, type, fun){
        if (target.removeEventListener) {
            target.removeEventListener(type, fun, false);
        }else if (target.detachEvent) {
            target.detachEvent("on" + type, fun);
        }else{
            target["on" + type] = null;
        };
    },
    //设置剪切板文本
    setClipboardText: function(event, value){
        if (event.clipboardData){
            return event.clipboardData.setData("text/plain", value);
        } else if (window.clipboardData){
            return window.clipboardData.setData("text", value);
        }
    },
    //阻止事件冒泡
    stopPropagation : function(event){
        if (event.stopPropagation) {
            event.stopPropagation();
        }else {
            event.cancelBubble = true;
        };
    }
};


//表单序列化
function serialize(form){
    var parts = [],
        field = null,
        i,
        len,
        j,
        optLen,
        option,
        optValue;
    for (i=0, len=form.elements.length; i < len; i++){
        field = form.elements[i];
        switch(field.type){
            case "select-one":
            case "select-multiple":
                if (field.name.length){
                    for (j=0, optLen = field.options.length; j < optLen; j++){option = field.options[j];
                        if (option.selected){
                            optValue = "";
                            if (option.hasAttribute){
                                optValue = (option.hasAttribute("value") ?
                                    option.value : option.text);
                            } else {
                                optValue = (option.attributes["value"].specified ?
                                    option.value : option.text);
                            }
                            parts.push(encodeURIComponent(field.name) + "=" +
                                encodeURIComponent(optValue));
                        }
                    }
                }
                break;
            case undefined: //字段集
            case "file": //文件输入
            case "submit": //提交按钮
            case "reset": //重置按钮
            case "button": //自定义按钮
                break;
            case "radio": //单选按钮
            case "checkbox": //复选框
                if (!field.checked){
                    break;
                }
            /* 执行默认操作 */
            default:
                //不包含没有名字的表单字段
                if (field.name.length){
                    parts.push(encodeURIComponent(field.name) + "=" +
                        encodeURIComponent(field.value));
                }
        }
    }
    return parts.join("&");
}

//绘图动画
function drawAnimation(img, ceil, context, canvas){
    this.img = img;
    this.sx = ceil.sx;
    this.sy = ceil.sy;
    this.swidth = ceil.swidth;
    this.sheight = ceil.sheight;
    this.left = ceil.left;
    this.dtop = ceil.top;
    this.width = ceil.width;
    this.height = ceil.height;
    this.canvas = canvas;
    this.context = context;
    this.context.clearRect(0,0,this.canvas.width,this.canvas.height);
    this.context.save();
    this.context.drawImage(this.img, this.sx, this.sy, this.swidth, this.sheight, this.left, this.dtop, this.width, this.height);
    this.context.restore();
}

// 当浏览器不支持requestAnimationFrame时，回退到之前的版本，即添加前缀，当浏览器不支持这个API时，使用setTimeout
(function() {
    var lastTime = 0;
    var vendors = ['webkit', 'moz'];
    for(var x = 0; x < vendors.length && !window.requestAnimationFrame; ++x) {
        window.requestAnimationFrame = window[vendors[x] + 'RequestAnimationFrame'];
        window.cancelAnimationFrame = window[vendors[x] + 'CancelAnimationFrame'] ||    // Webkit中此取消方法的名字变了
            window[vendors[x] + 'CancelRequestAnimationFrame'];
    }

    if (!window.requestAnimationFrame) {
        window.requestAnimationFrame = function(callback, element) {
            var currTime = new Date().getTime();
            var timeToCall = Math.max(0, 16.7 - (currTime - lastTime));
            var id = window.setTimeout(function() {
                callback(currTime + timeToCall);
            }, timeToCall);
            lastTime = currTime + timeToCall;
            return id;
        };
    }
    if (!window.cancelAnimationFrame) {
        window.cancelAnimationFrame = function(id) {
            clearTimeout(id);
        };
    }
}());

//清空className
function emptyClassName(arr){
    for(var i = 0; i < arr.length; i++){
        arr[i].className = "";
    }
}
//移除指定的className
function removeClassName(node, classStr){
    var index;
    var className = node.className;
    if(classStr == className){
        node.className = "";
    }else if((index = className.indexOf(classStr, 0)) >= 0){
        if(index == 0 && className.charAt(classStr.length) == " "){
            node.className = className.substr(classStr.length + 1, className.length -1);
        }else if(index > 0 && className.charAt(index + classStr.length) == " " && className.charAt(index - 1) == " "){
            var substr1 = className.substr(0, index - 2);
            var substr2 = className.substr(index + classStr.length + 1, className.length - 1);
            node.className = substr1 + " " + substr2;
        }else if(index > 0 && className.charAt(index - 1) == " " && ((index + classStr.length - 1) == (className.length - 1))){
            node.className = className.substr(0, index - 1);
        }else {
            node.className = className;
        }
    }else if((index = className.indexOf(classStr, 0)) == -1){
        node.className = className;
    }
}
//添加className
function addClassName(node, classStr){
    var index;
    var className = node.className;
    if(className.length == 0){//className 为空
        node.className = classStr;
    }else if(className.length > 0){//className 不为空
        if(className == classStr){//className 为要添加的class
            node.className = className;
        }else if((index = className.indexOf(classStr, 0)) == 0){//要添加的class出现在className第一个位置
            if(className.charAt(classStr.length) == " "){//className中包含了要添加的部分
                node.className = className;
            }else {//不包含
                node.className += classStr;
            }
        }else if((index = className.indexOf(classStr, 0)) > 0){
            if(className.charAt(index - 1) == " " && (className.charAt(index - 1 + classStr.length)) == " " || ((index - 1 + classStr.length) == className.length)){
                node.className = className;
            }
        }else if((index = className.indexOf(classStr, 0)) == -1){//不包含
            node.className += " " + classStr;
        }
    }
}


function removeArrClass(arr, className){
    for(var i = 0; i < arr.length; i++){
        removeClassName(arr[i], className);
    }
}

//禁止右键
//document.oncontextmenu = function () {
//    return false;
//};

var httpctx='http://api.xiniugame.com/';