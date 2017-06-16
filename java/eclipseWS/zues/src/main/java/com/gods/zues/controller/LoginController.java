package com.gods.zues.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.gods.zues.bean.User;
import com.gods.zues.service.UserService;

/**
 * Login Controller.
 * 
 * @author zhuysh
 *
 */
@RestController
public class LoginController {

    @Autowired
    private UserService userService;

    @RequestMapping("/login")
    public User login(User user) {
        return userService.login(user);
    }
}
