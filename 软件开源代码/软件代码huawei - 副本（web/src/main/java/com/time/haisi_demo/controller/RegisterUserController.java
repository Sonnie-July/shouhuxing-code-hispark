package com.time.haisi_demo.controller;

import com.alibaba.fastjson.JSONArray;
import com.time.haisi_demo.bean.User;
import com.time.haisi_demo.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class RegisterUserController {

    @Autowired
    private UserService userService;

    // 跳转至注册页面
    @GetMapping("/toRegisterPage")
    public String toRegisterPage() {
        return "register";
    }

    @ResponseBody
    @PostMapping("/checkRegisterUser")
    public String checkRegisterUser(String userName, String password, String tel) {
        User user = userService.registerUser(userName, password, tel);
        Object toJSON = JSONArray.toJSON(user);
        String toString = toJSON.toString();
//        System.out.println("注册返回的信息 ： " + toString);
        return toString;
    }

}
