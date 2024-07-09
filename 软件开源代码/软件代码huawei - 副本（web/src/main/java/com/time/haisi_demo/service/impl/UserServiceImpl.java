package com.time.haisi_demo.service.impl;

import com.time.haisi_demo.bean.User;
import com.time.haisi_demo.mapper.UserMapping;
import com.time.haisi_demo.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class UserServiceImpl implements UserService {

    @Autowired
    private UserMapping userMapping;

    @Override
    public User loginQueryUser(String userName, String password) {
        List<User> users = userMapping.loginQueryUser(userName, password, null);
        if (users.size() == 0) {
            // 未找到该用户，用户名或密码错误
            return new User(400,"登录失败!");
        } else {
            return new User(200,"登录成功!");
        }
    }

    @Override
    public User registerUser(String userName, String password, String tel) {
        // 注册用户： 将用户的信息保存到数据库前，先检查信息是否合法
        List<User> users = userMapping.loginQueryUser(userName, password, tel);

        if (users.size() == 0) {
            // 用户信息合法, 将用户信息存入数据库(插入成功返回值为 1， 失败为 0)
            int i = userMapping.registerUser(userName, password, tel);
            if (i > 0) {
                // 插入成功
                return new User(200, "注册成功");
            } else {
                // 插入失败
                return new User(400, "注册失败！");
            }
        } else {
            // 用户信息不合法（数据库中已经存在相同的用户名或密码）
            return new User(400, "用户名已存在或手机号以被注册！");
        }
    }
}
