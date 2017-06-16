package com.gods.zues.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.gods.zues.bean.User;
import com.gods.zues.service.UserService;

/**
 * User Controller
 * @author zhuysh
 *
 */
@RestController
@RequestMapping("/users")
public class UserController {

    @Autowired
    private UserService userService;
    
    @RequestMapping("/all")
    public List<User> getAllUsers() {
        return userService.getAllUsers();
    }
}
