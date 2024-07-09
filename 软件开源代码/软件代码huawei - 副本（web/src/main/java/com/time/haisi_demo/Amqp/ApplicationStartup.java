package com.time.haisi_demo.Amqp;

import javax.annotation.PostConstruct;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import com.time.haisi_demo.Amqp.ApplicationAMQPJavaDemoV1;

@Component
public class ApplicationStartup {

    private final ApplicationAMQPJavaDemoV1 amqpDemo;

    @Autowired
    public ApplicationStartup(ApplicationAMQPJavaDemoV1 amqpDemo) {
        this.amqpDemo = amqpDemo;
    }

    @PostConstruct
    public void init() {
        try {
            amqpDemo.getDocumentFromAmqp();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
