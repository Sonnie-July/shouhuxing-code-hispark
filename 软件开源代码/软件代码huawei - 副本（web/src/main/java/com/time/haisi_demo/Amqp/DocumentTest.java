package com.time.haisi_demo.Amqp;

public class DocumentTest {
    /**
     * {"deviceType":"CustomCategory","iotId":"I9OF9GoTbTfHB6dYTbXogmma00",
     * "requestId":"487558570","checkFailedData":{},"productKey":"gmmaQEeq1N6",
     * "gmtCreate":1648230109098,"deviceName":"LightSwitch",
     * "items":{"LightSwitch":{"value":1,"time":1648230109096},
     * "LightCurrent":{"value":1.5,"time":1648230109096},
     * "Brightness":{"value":86,"time":1648230109096},
     * "CurrentTemperature":{"value":27.5,"time":1648230109096}}}
     * @param args
     */
    public static void main(String[] args) {
        String str = "{\n" +
                "receive an message, the content is {\"resource\":\"device.property\",\"event\":\"report\",\"event_time\":\"20230504T130748Z\",\"request_id\":\"d734a1eb-88ce-4668-a109-2379c4d2fcbb\",\"notify_data\":{\"header\":{\"app_id\":\"81566639ad964d85b449b563812a17d4\",\"device_id\":\"640ec31492edbc7ee93a1a25_1683188885146\",\"node_id\":\"1683188885146\",\"product_id\":\"640ec31492edbc7ee93a1a25\",\"gateway_id\":\"640ec31492edbc7ee93a1a25_1683188885146\"},\"body\":{\"services\":[{\"service_id\":\"TrafficLight\",\"properties\":{\"温度\":5,\"AutoModule\":\"2\",\"湿度\":2,\"光照\":6,\"尿床\":6,\"人体红外\":7},\"event_time\":\"20230504T130748Z\"}]}}}";


        System.out.println();
    }

}
