package com.time.haisi_demo.bean;

public class CarData {

    private String power;
    private String temperature;
    private String humidity1;
    private String humidity2;
    public String lightNow;
    public String near;
    public String securityNow;
    public String bedWetting;
    public String updateNow;

    public CarData() {
    }

    public CarData(String power, String temperature, String humidity1, String humidity2, String lightNow, String near, String securityNow, String bedWetting, String updateNow) {
        this.power = power;
        this.temperature = temperature;
        this.humidity1 = humidity1;
        this.humidity2 = humidity2;
        this.lightNow = lightNow;
        this.near = near;
        this.securityNow = securityNow;
        this.bedWetting = bedWetting;
        this.updateNow = updateNow;
    }

    public String getPower() {
        return power;
    }

    public void setPower(String power) {
        this.power = power;
    }

    public String getTemperature() {
        return temperature;
    }

    public void setTemperature(String temperature) {
        this.temperature = temperature;
    }

    public String getHumidity1() {
        return humidity1;
    }

    public void setHumidity1(String humidity1) {
        this.humidity1 = humidity1;
    }

    public String getHumidity2() {
        return humidity2;
    }

    public void setHumidity2(String humidity2) {
        this.humidity2 = humidity2;
    }

    public String getLightNow() {
        return lightNow;
    }

    public void setLightNow(String lightNow) {
        this.lightNow = lightNow;
    }

    public String getNear() {
        return near;
    }

    public void setNear(String near) {
        this.near = near;
    }

    public String getSecurityNow() {
        return securityNow;
    }

    public void setSecurityNow(String securityNow) {
        this.securityNow = securityNow;
    }

    public String getBedWetting() {
        return bedWetting;
    }

    public void setBedWetting(String bedWetting) {
        this.bedWetting = bedWetting;
    }

    public String getUpdateNow() {
        return updateNow;
    }

    public void setUpdateNow(String updateNow) {
        this.updateNow = updateNow;
    }

    @Override
    public String toString() {
        return "CarData{" +
                "power='" + power + '\'' +
                ", temperature='" + temperature + '\'' +
                ", humidity1='" + humidity1 + '\'' +
                ", humidity2='" + humidity2 + '\'' +
                ", lightNow='" + lightNow + '\'' +
                ", near='" + near + '\'' +
                ", securityNow='" + securityNow + '\'' +
                ", bedWetting='" + bedWetting + '\'' +
                ", updateNow='" + updateNow + '\'' +
                '}';
    }
}
