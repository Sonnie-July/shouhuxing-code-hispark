package com.time.haisi_demo.Amqp;

import org.springframework.stereotype.Controller;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import java.io.IOException;

@Controller
public class DocumentManage extends HttpServlet {

    String mess;

    public void getDocFromAmqp(String message) throws ServletException, IOException {

        System.out.println("这是我的DocumentManage 中的数据 ： " + message);
        mess = message;
        System.out.println(mess);

    }

}
