$(function(){
var Page = {
    Init : function(){
        $("body").css("height",$(window).height());
        $("#loading").text("the future is now.");
        setTimeout(function(){
            $("#loading").fadeIn(1000);
        },1000);
    }
};

Page.Init();
});
