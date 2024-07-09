package com.time.haisi_demo.service;

import com.time.haisi_demo.bean.User;

public interface UserService {

    /**
     * 验证账号密码是否正确
     * 正确 ： 200
     * 错误 ： 400
     * @param userName
     * @param password
     * @return
     */
    public User loginQueryUser(String userName, String password);

    /**
     * 添加成功返回值 code为200， 失败 code为400
     * @param userName
     * @param password
     * @param email
     * @return
     */
    public User registerUser(String userName, String password, String tel);


}
