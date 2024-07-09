package com.time.haisi_demo.service;

import com.time.haisi_demo.bean.CarData;

public interface CarDataService {


    /**
     * 根据 id 查询车内数据
     *
     * @param id id
     * @return {@link CarData}
     */
    public CarData getOneData(int id);


}
