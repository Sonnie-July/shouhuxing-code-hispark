package com.time.haisi_demo.service.impl;

import com.time.haisi_demo.bean.CarData;
import com.time.haisi_demo.mapper.CarDataMapping;
import com.time.haisi_demo.service.CarDataService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class CarDataServiceImpl implements CarDataService {

    @Autowired
    private CarDataMapping carDataMapping;


    @Override
    public CarData getOneData(int id) {

        CarData data = carDataMapping.getOneData(id);

        return data;
    }
}
