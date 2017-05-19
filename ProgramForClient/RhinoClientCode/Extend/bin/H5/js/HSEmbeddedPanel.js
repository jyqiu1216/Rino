var HS_searchBtn = document.getElementsByClassName("HS_searchNow")[0];
var HS_searchText = HS_searchBtn.getElementsByClassName("HS_searchNowText")[0];
var waitTime = document.getElementsByClassName("HS_searchTime")[0].getElementsByTagName("span")[0];

var mm = 0;
var ss = 0;
var flag = 0;
var x=0,y=0,f=0,t1;

function searchMatch (){
    switch (flag){
        case 0 :
            flag = 1;
            $(".HS_searchNow img").attr("src","images/taskDekaron/HS_searchNow2.png");
            document.getElementsByClassName("HS_searchTime")[0].style.display = "block";
            t1=setInterval(time,1000);
            break;
        case 1 :
            flag = 0;mm = 0;ss = 0;x=0;y=0;clearInterval(t1);
            $(".HS_searchNow img").attr("src","images/taskDekaron/HS_searchNow1.png");
            waitTime.innerHTML = "00" + " " +":"+ " " + "00";
            document.getElementsByClassName("HS_searchTime")[0].style.display = "none";
            break;
    }
	
	sendMessageWS("START_GAME");
}

function time(){//计算秒
    x++;

    if(x<=9){
        ss='0'+x;
    }else if(x>=10&&x<=59){
        ss=x;
    }else if(x>59){
        f++;
        x=0;
        ss='00';
    }
    if(mm <= 999&&mm >= 100){
        mm=f;
    }else if(mm >= 10&&mm <= 99){
        mm='0'+f;
    }else if(mm < 10){
        mm='0'+f;
    }
    waitTime.innerHTML = mm + " " +":"+ " " +ss;
}

EventUtil.addHandler(HS_searchBtn,"click",searchMatch);