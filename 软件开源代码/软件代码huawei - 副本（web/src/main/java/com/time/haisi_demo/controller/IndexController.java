package com.time.haisi_demo.controller;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.time.haisi_demo.bean.User;
import com.time.haisi_demo.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;
import java.io.InputStreamReader;

@Controller
public class IndexController {

    @Autowired
    private UserService userService;

    // 跳转至首页
//    @RequestMapping("/")
//    public String toIndex() {
//        return "index";
//    }

    // 跳转至登录界面
    @GetMapping("/toLoginPage")
    public String toLoginPage() {
        return "login";
    }

//    跳转至车辆动态页面
    @GetMapping("/toCar_DynamicPage")
    public String toCar_DynamicPage() {
        return "car_dynamic";
    }

    @GetMapping("/toSafePage")
    public String toSafePage() {
        return "safe";
    }

    @GetMapping("/toCondiserPage")
    public String toCondiserPage() {
        return "condiser";
    }

    @GetMapping("/toFeedbackPage")
    public String tofeedbackPage() {
        return "feedback";
    }

    @GetMapping("/toBabyPage")
    public String toBabyPage() {
        return "baby";
    }

    @GetMapping("/toCar_PositionPage")
    public String toCar_PositionPage() {
        return "car_position";
    }
    /*
    * 用户登录，检查信息是否正确
    * */
    @ResponseBody
    @RequestMapping(value = "/checkLoginUser")
    public String checkLoginUser(String userName, String password) {
        User user = userService.loginQueryUser(userName, password);

        Object toJSON = JSONArray.toJSON(user);
        String toString = toJSON.toString();

//        System.out.println("将User对象转换为JSON ： " + toString);
        return toString;
    }
    @ResponseBody
    @RequestMapping(value = "/checkLoginUser1")
    public String checkLoginUser1(String userName, String password) {
        User user = userService.loginQueryUser(userName, password);

        Object toJSON = JSONArray.toJSON(user);
        String toString = toJSON.toString();
      if (user.getCode()==200){
          return "ok";
      }
//        System.out.println("将User对象转换为JSON ： " + toString);
        return "no";
    }

    // 跳转至登录成功后的首页
    @GetMapping("/toLogin_IndexPage")
    public String toLogin_IndexPage() {
        return "login_index";
    }


//    @ResponseBody
//    @RequestMapping (value = "/document_flush1")
//    public Object document_flush(HttpSession httpSession) {
//
//        return s;
//    }
    //
//    @ResponseBody
//    @GetMapping("/document_flush")
//    public Items document_flush(HttpSession httpSession) {
//         Object document =  httpSession.getAttribute("document");
////        System.out.println(document);
////        JSONObject s = JSONObject.parseObject((String) document);
//        String s = JSON.toJSONString(document);
//       //System.out.println(s);
////        JSONObject jsonObject = JSONObject.parseObject(document);
////
////        JSONObject p = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("Beep"));
////        System.out.println(p.getString("value"));
////        Items items = new Items();
////        items.setBeep(p.getString("value"));
////        String text= JSON.toJSONString(items);
//        JSONObject jsonObject = JSONObject.parseObject(String.valueOf(document));
//        JSONObject p1 = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("Beep"));
//
//        JSONObject p2 = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("LightLux"));
//
//        JSONObject p3 = JSONObject.parseObject(jsonObject.getJSONObject("items").getString("Humidity"));
//
//
//        Items items = new Items();
//        items.setBeep(p1.getString("value"));
//        items.setLightLux(p2.getString("value"));
//        items.setHumidity(p3.getString("value"));
////                items.setBeep("5");
////        items.setLightLux("5");
////        items.setHumidity("5");
//        items.setRainfall("5");
//        items.setIndoorTemperature("5");
//
//        return items;
//
//    }
    @GetMapping("/document")
    public String document(Model model, HttpSession httpSession) {
        return "HTML/device";
    }

    String s6;
        @ResponseBody
        @RequestMapping ("/receive")
        public Map<String, Object> propertyPush() {
            Map<String, Object> map = new HashMap<>();
            try {
                ServletRequestAttributes requestAttributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
                if (requestAttributes == null) {
                    map.put("HTTP", 500);
                    return map;
                }

                HttpServletRequest request = requestAttributes.getRequest();
                StringBuilder buffer = new StringBuilder();
                InputStream is = request.getInputStream();
                InputStreamReader reader = new InputStreamReader(is);
                BufferedReader br = new BufferedReader(reader);
                String s = "";
                while ((s = br.readLine()) != null) {
                    buffer.append(s);
                }
                String result = buffer.toString();
                s6=buffer.toString();
                // 在这里处理接收到的消息 result
                System.out.println("***********************");
                System.out.println(result);
                map.put("HTTP", 200);
                return map;
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    @ResponseBody
    @RequestMapping(value ="/receive1" )
    public String document_flush1() {


        return s6;
        // return "测试数据";
    }
}
