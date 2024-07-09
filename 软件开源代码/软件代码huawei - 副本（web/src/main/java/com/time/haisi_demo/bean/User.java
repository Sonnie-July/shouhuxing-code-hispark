package com.time.haisi_demo.bean;

public class User {

    private String userName;

    private String password;

    private String tel;

    private Integer code;

    private String state;

    public User() {
    }

    public User(Integer code, String state) {
        this.code = code;
        this.state = state;
    }

    public User(String userName, String password, String tel) {
        this.userName = userName;
        this.password = password;
        this.tel = tel;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getTel() {
        return tel;
    }

    public void setTel(String tel) {
        this.tel = tel;
    }

    public Integer getCode() {
        return code;
    }

    public void setCode(Integer code) {
        this.code = code;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    @Override
    public String toString() {
        return "User{" +
                "userName='" + userName + '\'' +
                ", password='" + password + '\'' +
                ", tel='" + tel + '\'' +
                ", code=" + code +
                ", state='" + state + '\'' +
                '}';
    }
}
