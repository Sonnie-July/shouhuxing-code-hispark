package com.time.haisi_demo;

import com.time.haisi_demo.bean.CarData;
import com.time.haisi_demo.bean.User;
import com.time.haisi_demo.mapper.CarDataMapping;
import com.time.haisi_demo.mapper.UserMapping;
import com.time.haisi_demo.service.CarDataService;
import com.time.haisi_demo.service.UserService;
import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.List;

@Slf4j
@SpringBootTest
class HaiSiDemoApplicationTests {

    @Autowired
    UserService userService;

    @Autowired
    UserMapping userMapping;

    @Autowired
    CarDataMapping carData;

    @Autowired
    CarDataService carDataService;

    @Test
    void contextLoads() {
    }

    @Test
    public void text01() {
        User admin = userService.loginQueryUser("admin", "123456");
        System.out.println("--------- " + admin);
        log.info("返回的数据为 ： {}" + admin);
    }

    @Test
    public void text02() {
        User tom = userService.registerUser("tom", "123456", "13004651780");
        System.out.println("--------- " + tom);
        log.info("返回的数据为 ： {}" + tom);
    }

    @Test
    public void  test03() {
        CarData data = carData.getOneData(3);
        log.info("查询到的数据为： {}" + data);
    }

    @Test
    public void  test04() {
        CarData oneData = carDataService.getOneData(2);
        log.info("查询到的数据为： {}" + oneData);
    }
}
