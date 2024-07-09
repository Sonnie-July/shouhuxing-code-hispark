package com.time.haisi_demo.Amqp;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.time.haisi_demo.bean.Items;

import java.util.Collections;

public class ceshe{
    public static void main( String[] args ){

        String str="{\"resource\":\"device.property\",\"event\":\"report\",\"event_time\":\"20230504T132419Z\",\"request_id\":\"410c2b42-6e65-4c56-9120-de7e6e31aa93\",\"notify_data\":{\"header\":{\"app_id\":\"81566639ad964d85b449b563812a17d4\",\"device_id\":\"640ec31492edbc7ee93a1a25_1683188885146\",\"node_id\":\"1683188885146\",\"product_id\":\"640ec31492edbc7ee93a1a25\",\"gateway_id\":\"640ec31492edbc7ee93a1a25_1683188885146\"},\"body\":{\"services\":[{\"service_id\":\"TrafficLight\",\"properties\":{\"Temperature\":25,\"AutoModule\":\"10\",\"Humidity\":4,\"Illumination\":213,\"Bed-Wetting\":13,\"Human-infrared\":13},\"event_time\":\"20230504T132419Z\"}]}}}";
//        JSONArray jsonArray = new JSONArray(params);
//
//        JSONObject p = JSONObject.parseObject();
//        System.out.println(jsonObject.getJSONArray("items"));
        JSONObject jsonObject = JSONObject.parseObject(str);
       // JSONObject p1 = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("Beep"));

//        JSONObject p2 = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("LightLux"));
//
//        JSONObject p3 = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("Humidity"));

//        Items items = new Items();
//        items.setBeep(p1.getString("value"));
//        items.setLightLux(p2.getString("value"));
//        items.setHumidity(p3.getString("value"));
//        String text= JSON.toJSONString(items);
       JSONObject p1= JSONObject.parseObject(jsonObject.getJSONObject("notify_data").getString("body"));

       String p =p1.getString("services");
       String s = p.substring(1,p.length()-1);
       JSONObject p2 =JSONObject.parseObject(s);

        System.out.println(p2.getJSONObject("properties").getString("Temperature"));
    }
}