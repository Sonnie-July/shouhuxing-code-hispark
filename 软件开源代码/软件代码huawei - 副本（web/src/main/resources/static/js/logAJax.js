//获取按钮元素
var btn = document.getElementById("btn");
// 添加点击事件
btn.onclick = function () {

    Ajaxset();
    // return username1;
}

// 添加回车键事件
document.onkeydown = function (e) {
    // 兼容FF和IE和Opera
    var theEvent = window.event || e;
    var code = theEvent.keyCode || theEvent.which || theEvent.charCode;
    if (code == 13) {
        Ajaxset();
        // return username1;
    }
}

// 封装AJax函数
function Ajaxset() {
    // 获取input输入值
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;

    console.log("username : " + username + "password : " + password)

    // 发送Ajax到登录接口
    var xhr = new XMLHttpRequest();
    xhr.withCredentials = true;
    // 监控Ajax返回值
    xhr.addEventListener("readystatechange", function () {
        if (this.readyState == 4 && this.status == 200) {
            var text = this.responseText; //使用接口返回内容，响应内容
            var resultJson = eval("(" + text + ")"); //把响应内容对象转成javascript对象，便于后期数据引用
            console.log(resultJson);
            var code = resultJson.code;//获取json中的code键对应的值

            if (code !== 200) { //登录失败状况

                alert("用户名或密码错误");

            } else {

                alert("登录成功！");
                // 跳转到主页面
                 //window.location.href="http://49.233.245.182:9999/toLogin_IndexPage"
                //window.location.href="http://192.168.43.232:9999/toLogin_IndexPage"
                window.location.href="http://localhost:9999/toLogin_IndexPage"
                // window.location.href="http://192.168.113.40:9999/toLogin_IndexPage"
                //window.location.href="http://192.168.200.110:9999/toLogin_IndexPage"
               // window.location.href="http://192.168.6.128:9999/toLogin_IndexPage"
            } //登录成功
        }
    });

    // Ajax设置
     //xhr.open("POST", "http://49.233.245.182:9999/checkLoginUser");
    xhr.open("POST", "http://localhost:9999/checkLoginUser");
    //xhr.open("POST", "http://192.168.43.232:9999/checkLoginUser");
    // xhr.open("POST", "http://192.168.113.40:9999/checkLoginUser");
    //xhr.open("POST", "http://192.168.200.110:9999/checkLoginUser");
    //xhr.open("POST", "http://192.168.6.128:9999/checkLoginUser");
    xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded")
    xhr.send("userName=" + username + "&password=" + password)


    // var data = { "userName": username, "password": password };
    // xhr.open("POST", "http://localhost:9999/loginUser?userName=admin&password=123456");  GET方式，参数拼接在地址后，且不需要setRequestHeader
    // xhr.setRequestHeader("Content-Type", "application/json;charset=utf-8");
    // xhr.send(JSON.stringify(data));//将字符串转化为json数据
}

