package com.gods.zues.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.gods.zues.bean.User;
import com.gods.zues.mapper.UserMapper;

/**
 * User service.
 * @author zhuysh
 *
 */
@Service
public class UserService {

    @Autowired
    private UserMapper userMapper;
    
    /**
     * User login.
     * @param user
     * @return
     */
    public User login(User user){
        return userMapper.login(user);
    }
    
    /**
     * get all users
     * @return
     */
    public List<User> getAllUsers() {
        return userMapper.getAllUsers();
    }
}
