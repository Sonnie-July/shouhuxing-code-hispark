// 只有三个ture时才执行Ajax操作
var flag1 = false;
var flag2 = false;
var flag3 = false;
var flag4 = false;

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
        flag1 = true;
        document.getElementById("check1").innerHTML = '格式正确';
    }
}

// 手机号检查
var input2 = document.getElementsByTagName('input')[1];
input2.oninput = function () {
    var reg = /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/;
    if (reg.test(this.value)) {
        flag2 = true;
        document.getElementById("check2").innerHTML = '格式正确';
    } else {
        document.getElementById("check2").innerHTML = '请输入正确的手机号格式';
    }
}

// 密码检查
var input3 = document.getElementsByTagName('input')[2];
input3.oninput = function () {
    if (this.value == "") {
        document.getElementById("check3").innerHTML = '密码不能为空';
    }
    else if ((this.value.length < 5) || (this.value.length > 20)) {
        document.getElementById("check3").innerHTML = '密码必须为5~20位';
    }
    else {
        flag3 = true;
        document.getElementById("check3").innerHTML = '格式正确';
    }
}

// 确认密码检查
var input4 = document.getElementsByTagName('input')[3];
input4.oninput = function () {
    if (this.value != input3.value) {
        document.getElementById("check4").innerHTML = '密码不一致';
    }
    else {
        flag4 = true;
        document.getElementById("check4").innerHTML = '';
    }
}