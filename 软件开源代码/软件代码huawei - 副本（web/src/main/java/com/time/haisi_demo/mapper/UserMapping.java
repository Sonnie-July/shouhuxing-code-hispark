package com.time.haisi_demo.mapper;

import com.time.haisi_demo.bean.User;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

@Mapper
public interface UserMapping {

    // 登录验证 & 注册验证
    public List<User> loginQueryUser(@Param("userName") String userName, @Param("password") String password, @Param("tel") String tel);

    // 添加注册用户的信息
    public int registerUser(@Param("userName") String userName, @Param("password") String password, @Param("tel") String tel);


}
