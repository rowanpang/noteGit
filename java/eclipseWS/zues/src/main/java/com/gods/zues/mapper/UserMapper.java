package com.gods.zues.mapper;

import java.util.List;

import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import com.gods.zues.bean.User;

/**
 * User Mapper
 * 
 * @author zhuysh
 *
 */
public interface UserMapper {

    /**
     * LOGIN IN check。
     * 
     * @param userName
     * @param password
     * @return
     */
    @Select("SELECT * FROM USER WHERE USERNAME = #{user.userName} AND PASSWORD = #{user.password}")
    User login(@Param("user") User user);

    /**
     * get all users list
     * @return
     */
    @Select("SELECT * FROM TB_USER")
    List<User> getAllUsers();
}
