package com.time.haisi_demo.Amqp;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.time.haisi_demo.bean.Items;
import org.apache.qpid.jms.JmsConnection;
import org.apache.qpid.jms.JmsConnectionFactory;
import org.apache.qpid.jms.JmsConnectionListener;
import org.apache.qpid.jms.message.JmsInboundMessageDispatch;
import org.apache.qpid.jms.transports.TransportOptions;
import org.apache.qpid.jms.transports.TransportSupport;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.jms.*;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.servlet.http.HttpSession;
import java.net.URI;
import java.util.Hashtable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

@Controller
public class ApplicationAMQPJavaDemoV1 {
    Items items = new Items();
    String s1;
    //异步线程池，参数可以根据业务特点作调整，也可以用其他异步方式来处理。
    private final static ExecutorService executorService = new ThreadPoolExecutor(
            Runtime.getRuntime().availableProcessors(), Runtime.getRuntime().availableProcessors() * 2,
            60, TimeUnit.SECONDS, new LinkedBlockingQueue<>(10000));//修改过


//    @GetMapping("/")
    public String getDocumentFromAmqp () throws Exception {
        //连接凭证接入键值。
        String accessKey = "Xps6oj3y";
        long timeStamp = System.currentTimeMillis();
        //UserName组装方法，请参见文档：AMQP客户端接入说明。
        String userName = "accessKey=" + accessKey + "|timestamp=" + timeStamp;
        //连接凭证接入码。
        String password = "qxZzls1kmEv6CYQs2rKqDqkZ7Y3IVgqU";
        //按照qpid-jms的规范，组装连接URL。
        String baseUrl = "72bbfd1211.iot-amqps.cn-north-4.myhuaweicloud.com";
        String connectionUrl = "amqps://" + baseUrl + ":5671?amqp.vhost=default&amqp.idleTimeout=8000&amqp.saslMechanisms=PLAIN";
        Hashtable<String, String> hashtable = new Hashtable<>();
        hashtable.put("connectionfactory.HwConnectionURL", connectionUrl);
        //队列名，可以使用默认队列DefaultQueue
        String queueName = "DefaultQueue";
        hashtable.put("queue.HwQueueName", queueName);
        hashtable
                .put(Context.INITIAL_CONTEXT_FACTORY, "org.apache.qpid.jms.jndi.JmsInitialContextFactory");
        Context context = new InitialContext(hashtable);
        JmsConnectionFactory cf = (JmsConnectionFactory) context.lookup("HwConnectionURL");
        //同一个链接可创建多个queue,与前面queue.HwQueueName作好配对就行
        Destination queue = (Destination) context.lookup("HwQueueName");

        //信任服务端
        TransportOptions to = new TransportOptions();
        to.setTrustAll(true);
        cf.setSslContext(TransportSupport.createJdkSslContext(to));

        // 创建连接
        Connection connection = cf.createConnection(userName, password);
        ((JmsConnection) connection).addConnectionListener(myJmsConnectionListener);
        // 创建 Session
        // Session.CLIENT_ACKNOWLEDGE: 收到消息后，需要手动调用message.acknowledge()。
        // Session.AUTO_ACKNOWLEDGE: SDK自动ACK（推荐）。
        Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
        connection.start();
        // 创建 Receiver Link
        MessageConsumer consumer = session.createConsumer(queue);
        //处理消息有两种方式
        // 1，主动拉数据（推荐），参照receiveMessage(consumer)
        // 2, 添加监听，参照consumer.setMessageListener(messageListener), 服务端主动推数据给客户端，但得考虑接受的数据速率是客户能力能够承受住的
    //receiveMessage(consumer);
        // consumer.setMessageListener(messageListener);
        consumer.setMessageListener(new MessageListener() {

            @Override
            public void onMessage(Message message) {
                try {
                    // 建议异步处理收到的消息，确保onMessage函数里没有耗时逻辑。
                    // 如果业务处理耗时过程过长阻塞住线程，可能会影响SDK收到消息后的正常回调。
                    executorService.submit(new Runnable() {

                        @Override
                        public void run() {

                            /**
                             * 在这里处理您收到消息后的具体业务逻辑。
                             */
                            try {
                                String body = message.getBody(String.class);
                                String content = new String(body);
                                JSONObject jsonObject = JSONObject.parseObject(content);

                                JSONObject p1= JSONObject.parseObject(jsonObject.getJSONObject("notify_data").getString("body"));

                                String p =p1.getString("services");
                                String s = p.substring(1,p.length()-1);
                                JSONObject p2 =JSONObject.parseObject(s);

                               // Items items = new Items();

                                if(p2.getJSONObject("properties").getString("Temperature")!=null){
                                    items.setTemperature(p2.getJSONObject("properties").getString("Temperature"));
                                }
                                if(p2.getJSONObject("properties").getString("emojistatus")!=null){
                                    items.setEmojistatus(p2.getJSONObject("properties").getString("emojistatus"));
                                    if (items.getEmojistatus().equals("1")){
                                        items.setEmoji("哭");
                                    }
                                    if (items.getEmojistatus().equals("2")){
                                        items.setEmoji("睡");
                                    }
                                    if (items.getEmojistatus().equals("3")){
                                        items.setEmoji("醒");
                                    }
                                    if (items.getEmojistatus().equals("0")){
                                        items.setEmoji("无");
                                    }
                                }
                                if(p2.getJSONObject("properties").getString("Humidity")!=null){
                                    items.setHumidity(p2.getJSONObject("properties").getString("Humidity"));
                                }
                                if(p2.getJSONObject("properties").getString("Bed-Wetting")!=null){
                                    items.setBedWetting(p2.getJSONObject("properties").getString("Bed-Wetting"));
                                    if (Double.parseDouble(items.getBedWetting())<1.6){
                                        items.setNiaChung("否");

                                    }
                                    else {
                                        items.setNiaChung("是");
                                    }
                                }
                                if(p2.getJSONObject("properties").getString("Illumination")!=null){
                                    Double value = Double.parseDouble(p2.getJSONObject("properties").getString("Illumination"));
                                    items.setIllumination(String.format("%.2f", value).toString());

                                    if (Double.parseDouble(items.getIllumination())>300){
                                        items.setZheyang("打开");
                                    }
                                    if (Double.parseDouble(items.getIllumination())<100){
                                        items.setZheyang("关闭");
                                    }

                                }

                                if(p2.getJSONObject("properties").getString("Human-infrared")!=null){
                                    items.setHumaninfrared(p2.getJSONObject("properties").getString("Human-infrared"));
                                    if (Double.parseDouble(items.getHumaninfrared())<1.5){
                                        items.setRenti("否");

                                    }
                                    else {
                                        items.setRenti("是");
                                    }
                                }

//                                if (Integer.parseInt(items.getHumaninfrared())<2.5){
//                                    items.setRenti("否");
//
//                                }
//                                else {
//                                    items.setRenti("是");
//                                }

//                                if (Integer.parseInt(items.getBedWetting())>3.3){
//                                    items.setNiaChung("否");
//
//                                }
//                                else {
//                                    items.setNiaChung("是");
//                                }
//                                items.setTemperature(p2.getJSONObject("properties").getString("Temperature"));
//                                items.setHumidity(p2.getJSONObject("properties").getString("Humidity"));
//                                items.setBedWetting(p2.getJSONObject("properties").getString("Bed-Wetting"));
//                                items.setIllumination(p2.getJSONObject("properties").getString("Illumination"));
//                                items.setHumaninfrared(p2.getJSONObject("properties").getString("Human-infrared"));
       if(items.getTemperature()!=null&&items.getHumidity()!=null&&items.getBedWetting()!=null&&items.getHumaninfrared()!=null&&items.getIllumination()!=null&&items.getRenti()!=null&&items.getNiaChung()!=null){

                                   // sessions.setAttribute("document",items);
           s1=JSON.toJSONString(items);
                                }
                               // meeg(sessions);


                                System.out.println("receive an message, the content is " + content);
                            } catch (Exception e) {
                                System.out.println("processMessage occurs error: " + e.getMessage());
                                e.printStackTrace();
                            }
                        }
                    });
                } catch (Exception e) {
                    System.out.println("submit task occurs exception: " + e.getMessage());
                    e.printStackTrace();
                }
            }
        });
       return "/login";
    }
//    String s;
//    public void meeg(HttpSession httpSession){
//        Object document = httpSession.getAttribute("document");
//        System.out.println(document);
//        s = JSON.toJSONString(document);
//        System.out.println(s);
//    }

    public void me(){
        try {
            getDocumentFromAmqp();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @ResponseBody
    @RequestMapping("/document_flush")
    public Items document_flush() {

        return items;
    }
    @ResponseBody
    @RequestMapping(value = "/document_flush1")
    public String document_flush1() {

        System.out.println(s1);
      return s1;
       // return "测试数据";
    }
    /**
     * 在这里处理您收到消息后的具体业务逻辑。
     */


    JmsConnectionListener myJmsConnectionListener = new JmsConnectionListener() {
        /**
         * 连接成功建立。
         */
        @Override
        public void onConnectionEstablished(URI remoteURI) {
            System.out.println("onConnectionEstablished, remoteUri:" + remoteURI);
        }

        /**
         * 尝试过最大重试次数之后，最终连接失败。
         */
        @Override
        public void onConnectionFailure(Throwable error) {
            System.out.println("onConnectionFailure, " + error.getMessage());
        }

        /**
         * 连接中断。
         */
        @Override
        public void onConnectionInterrupted(URI remoteURI) {
            System.out.println("onConnectionInterrupted, remoteUri:" + remoteURI);
        }

        /**
         * 连接中断后又自动重连上。
         */
        @Override
        public void onConnectionRestored(URI remoteURI) {
            System.out.println("onConnectionRestored, remoteUri:" + remoteURI);
        }

        @Override
        public void onInboundMessage(JmsInboundMessageDispatch envelope) {
            System.out.println("onInboundMessage, " + envelope);
        }

        @Override
        public void onSessionClosed(Session session, Throwable cause) {
            System.out.println("onSessionClosed, session=" + session + ", cause =" + cause);
        }

        @Override
        public void onConsumerClosed(MessageConsumer consumer, Throwable cause) {
            System.out.println("MessageConsumer, consumer=" + consumer + ", cause =" + cause);
        }

        @Override
        public void onProducerClosed(MessageProducer producer, Throwable cause) {
            System.out.println("MessageProducer, producer=" + producer + ", cause =" + cause);
        }
    };
}
