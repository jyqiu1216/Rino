var LOLStakeBox = document.getElementsByClassName("LOL_stakeBox")[0];
var gaming = false;
function LOLStakeObj (obj){
    this.obj = obj;
    this.stakePointsPanel = null;
    this.stakePointsList = null;
    this.stakePointsLi = null;
    this.stakeBtn = null;
    this.stakeBox = null;
    this.shrinkBtn = null;
    this.detaliBox = null;
    this.retract = null;
}
LOLStakeObj.prototype = {
    getDetailBox : function (){
        this.detaliBox = document.getElementsByClassName("LOL_stakeBoxDetail")[0];
    },
    getRetract :function (){
        this.retract = document.getElementsByClassName("LOL_stakeBoxRetract")[0];
    },
    getStakePointsPanel : function (){
        this.stakePointsPanel = document.getElementsByClassName("LOL_stakePoints")[0];
    },
    getStakePointsList :function (){
        this.stakePointsList = this.stakePointsPanel.getElementsByTagName("ul")[0];
    },
    getStakePointsLi :function (){
        this.stakePointsLi = this.stakePointsList.getElementsByTagName("li");
        for(var i = 0; i < this.stakePointsLi.length; i++){
            this.stakePointsLi[i].setAttribute("beClick","false");
        }
    },
    getStakeBtn :function (){
        this.stakeBtn = this.stakePointsPanel.getElementsByClassName("LOL_stakeBtn")[0];
        this.stakeBtn.setAttribute("beStake","false");
    },
    getStakeBox : function (){
        this.stakeBox = document.getElementsByClassName("LOL_stakeBoxRetract")[0];
    },
    getShrinkBtn :function (){
        this.shrinkBtn = $(".LOL_shrinkBtn");
    },
    stakeListLiClick :function (){
        var me = this;
        for(var i = 0; i< this.stakePointsLi.length; i++){
            this.stakePointsLi[i].onclick = function (){
                var stakeLi = me.stakeBtn.getAttribute("beStake");
                if(stakeLi == "false"){
                    var clickLi = this.getAttribute("beClick");
                    var chooseA = this.getElementsByClassName("LOL_chosen")[0];
                    if(clickLi == "false"){
                        this.setAttribute("beClick","true");
                        $(this).siblings().attr("beClick","false");
                        $(".LOL_chosen").hide();
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
            $(this).html("已押注");
        }
    },
    detailPanelHide :function (){
        var me = this;
        for(var i = 0; i<this.shrinkBtn.length; i ++){
            this.shrinkBtn[i].onclick = function (){
                console.log(1);
                me.detaliBox.className = "LOL_stakeBoxDetail LOLDetailPackUp";
                setTimeout(function(){
                    me.retract.className = "LOL_stakeBoxRetract LOLDetailShowDown"
                },1000)
            }
        }
    },
    detailPanelShow :function (){
        var me = this;
        this.retract.onclick = function (){
            this.className = "LOL_stakeBoxRetract LOLDetailShowUp";
            setTimeout(function (){
                me.detaliBox.className = "LOL_stakeBoxDetail LOLDetailPackDown";
            },1000)
        }
    },
    init :function (){
        this.getDetailBox();
        this.getRetract();
        this.getStakePointsPanel();
        this.getStakePointsList();
        this.getStakePointsLi();
        this.getStakeBtn();
        this.getStakeBox();
        this.getShrinkBtn();
        this.stakeListLiClick();
        this.stake();
        this.detailPanelHide();
        this.detailPanelShow();
    }
};
var LOLStake = new LOLStakeObj(LOLStakeBox);
LOLStake.init();