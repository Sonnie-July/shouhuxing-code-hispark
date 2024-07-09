
//获取按钮元素
var btn = document.getElementById("btn");
// 添加点击事件
btn.onclick = function () {
    Ajaxset();
}

// 添加回车键事件
document.onkeydown = function (e) {
    // 兼容FF和IE和Opera
    var theEvent = window.event || e;
    var code = theEvent.keyCode || theEvent.which || theEvent.charCode;
    if (code == 13) {
        Ajaxset();
    }
}
// 封装Ajax函数
function Ajaxset() {
    // 需要3个输入框都有内容才能执行函数
    if (flag1 == true && flag2 == true && flag3 == true && flag4 == true) {
        // 获取input输入值
        var tel = document.getElementById('tel').value;
        var username = document.getElementById('username').value;
        var password = document.getElementById('password').value;
        //发送Ajax到注册接口
        var xhr = new XMLHttpRequest();
        xhr.withCredentials = true;
        // 监控Ajax返回值
        xhr.addEventListener("readystatechange", function () {
            if (this.readyState === 4 && this.status === 200) {
                var text = this.responseText; //使用接口返回内容，响应内容
                var resultJson = eval("(" + text + ")"); //把响应内容对象转成javascript对象
                console.log(resultJson);
                var code = resultJson.code;//获取json中的code键对应的值

                if (code != 200) {
                    alert("该用户已存在");
                }
                else {
                    alert("恭喜您，注册成功！");
                    // 跳转至登录页面
                     //window.location.href="http://49.233.245.182:9999/toLoginPage"
                    window.location.href="http://localhost:9999/toLoginPage"
                    // window.location.href="http://192.168.113.40:9999/toLoginPage"
                    //window.location.href="http://192.168.200.110:9999/toLoginPage"
                    //window.location.href="http://192.168.6.128:9999/toLoginPage"
                }
            }
        });

         //xhr.open("POST", "http://49.233.245.182:9999/checkRegisterUser");
        xhr.open("POST", "http://localhost:9999/checkRegisterUser");
        // xhr.open("POST", "http://192.168.113.40:9999/checkRegisterUser");
        //xhr.open("POST", "http://192.168.200.110:9999/checkRegisterUser");
        //xhr.open("POST", "http://192.168.6.128:9999/checkRegisterUser");
        xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded")
        xhr.send("userName=" + username + "&password=" + password + "&tel=" + tel)

        // var data = { "email": email, "username": username, "password": password };
        // xhr.open("POST", "http://175.178.51.126:8091/smallA/register");
        // xhr.setRequestHeader("Content-Type", "application/json;charset=utf-8");
        // xhr.send(JSON.stringify(data));//将字符串转化为json数据
    }
    else {
        alert("信息输入不完整");
    }
}