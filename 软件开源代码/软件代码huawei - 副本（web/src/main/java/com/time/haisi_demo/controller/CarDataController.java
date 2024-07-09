package com.time.haisi_demo.controller;

import com.time.haisi_demo.bean.CarData;
import com.time.haisi_demo.service.CarDataService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class CarDataController {

    @Autowired
    private CarDataService carDataService;

    /**
     * 数据库查询数据，用于页面数据的实时刷新
     *
     * @param id id
     * @return {@link CarData}
     */
    @ResponseBody
    @GetMapping("/CarData_Flash")
    public CarData carData_Flash(@RequestParam(value = "id", defaultValue = "1") int id) {
        CarData data = carDataService.getOneData(id);
        return data;
    }

}
