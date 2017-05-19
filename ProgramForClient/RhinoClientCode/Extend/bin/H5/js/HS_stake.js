var HSStakeBox = document.getElementsByClassName("HS_stakeBox")[0];
function HSEmbedded(obj){
    this.obj = obj;
    this.retract = null;
    this.detailBox = null;
    this.pointsPanel = null;
    this.stakeList = null;
    this.stakeListLi = null;
    this.stakeBtn = null;
    this.hideBtn = null;
}
HSEmbedded.prototype = {
    getRetract: function (){
        this.retract = document.getElementsByClassName("HS_stakeBoxRetract")[0];
    },
    getDetailBox: function (){
        this.detaliBox = document.getElementsByClassName("HS_stakeBoxDetail")[0];
    },
    getPointsPanel :function (){
        this.pointsPanel = document.getElementsByClassName("HS_stake")[0];
    },
    getStakeList :function (){
        this.stakeList = this.pointsPanel.getElementsByTagName("ul")[0];
    },
    getStakeListLi :function (){
        this.stakeListLi = this.stakeList.getElementsByTagName("li");
        for(var i = 0; i< this.stakeListLi.length; i++){
            this.stakeListLi[i].setAttribute("beClick","false");
        }
    },
    getStakeBtn : function (){
        this.stakeBtn = document.getElementsByClassName("stakeBtn")[0];
        this.stakeBtn.setAttribute("beStake","false");
    },
    getHideBtn : function (){
        this.hideBtn = document.getElementsByClassName("HS_packUp")[0];
    },
    stakeListLiClick :function (){
        var me = this;
        for(var i = 0; i< this.stakeListLi.length; i++){
            this.stakeListLi[i].onclick = function (){
                var stakeLi = me.stakeBtn.getAttribute("beStake");
                if(stakeLi == "false"){
                    var clickLi = this.getAttribute("beClick");
                    var chooseA = this.getElementsByClassName("HS_chosen")[0];
                    if(clickLi == "false"){
                        this.setAttribute("beClick","true");
                        $(this).siblings().attr("beClick","false");
                        $(".HS_chosen").hide();
                        $(chooseA).show();
                    }else {
                        this.setAttribute("beClick","false");
                        $(chooseA).hide();
                    }
                }
            }
        }
    },
    stake : function (){
        this.stakeBtn.onclick = function (){
            this.setAttribute("beStake","true");
            $(this).addClass("active");
        }
    },
    detailPanelHide : function (){
        var me = this;
        this.hideBtn.onclick = function (){
            me.detaliBox.className = "HS_stakeBoxDetail HSDetailPackUp";
            setTimeout(function(){
                me.retract.className = "HS_stakeBoxRetract HSDetailShowDown"
            },1000)
        }
    },
    detailPanelShow :function (){
        var me = this;
        this.retract.onclick = function (){
            this.className = "HS_stakeBoxRetract HSDetailShowUp";
            setTimeout(function (){
                me.detaliBox.className = "HS_stakeBoxDetail HSDetailPackDown";
            },1000)
        }
    },
    init: function(){
        this.getRetract();
        this.getDetailBox();
        this.getPointsPanel();
        this.getStakeList();
        this.getStakeListLi();
        this.getStakeBtn();
        this.getHideBtn();
        this.stake();
        this.stakeListLiClick();
        this.detailPanelHide();
        this.detailPanelShow();
    }
};
var HS = new HSEmbedded(HSStakeBox);
HS.init();