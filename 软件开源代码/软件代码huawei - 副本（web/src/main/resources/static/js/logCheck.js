// 账号检查
var input1 = document.getElementsByTagName('input')[0];
input1.oninput = function () {
    if (this.value == "") {
        document.getElementById("check1").innerHTML = '用户名不能为空';
    }
    else if ((this.value.length < 5) || (this.value.length > 20)) {
        document.getElementById("check1").innerHTML = '用户名必须为5~20位';
    }
    else {
        
        document.getElementById("check1").innerHTML = '格式正确';
    }
}

// 密码检查
var input2 = document.getElementsByTagName('input')[1];
input2.oninput = function () {
    if (this.value == "") {
        document.getElementById("check2").innerHTML = '密码不能为空';
    }
    else if ((this.value.length < 5) || (this.value.length > 20)) {
        document.getElementById("check2").innerHTML = '密码必须为5~20位';
    }
    else {
        document.getElementById("check2").innerHTML = '格式正确';
    }
}