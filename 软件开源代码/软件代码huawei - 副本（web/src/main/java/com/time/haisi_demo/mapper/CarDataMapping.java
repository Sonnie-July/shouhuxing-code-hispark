package com.time.haisi_demo.mapper;

import com.time.haisi_demo.bean.CarData;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

@Mapper
public interface CarDataMapping {

    /**
     *     根据 id 查询数据
     *
     * @param id id
     * @return {@link CarData}
     */
    public CarData getOneData(@Param("id") int id);

}
