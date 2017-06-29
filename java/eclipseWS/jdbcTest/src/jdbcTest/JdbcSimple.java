package jdbcTest;

import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

public class JdbcSimple {

    public static void main(String[] args) throws SQLException {
	System.out.println("Hello world");

	try {
	    Class.forName("org.sqlite.JDBC");
	} catch (ClassNotFoundException e1) {
	    System.err.println("not fount sqlite jdbc driver");
	    e1.printStackTrace();
	}
	String url = "jdbc:sqlite:./simpleTest.sqlite";
	Connection conn = null;
	try {
	    conn = DriverManager.getConnection(url);
	} catch (SQLException e) {
	    e.printStackTrace();
	}
	Statement stmt = conn.createStatement();
	String sql = null;
	sql = "drop table if exists people";
	stmt.executeUpdate(sql);
	sql = "create table if not exists people(name varchar(20),age integer,addr varchar(100))";
	stmt.executeUpdate(sql);
	sql = "insert into people values('rowan',30,'Jinan,ShanDong province')";
	stmt.executeUpdate(sql);
	
	sql = "insert into people values(?,?,?)";
	PreparedStatement prepStmt = conn.prepareStatement(sql);
	prepStmt.setString(1, "lvzhl");
	prepStmt.setInt(2, 30);
	prepStmt.setString(3, "WanKe");
	prepStmt.addBatch();

	prepStmt.setString(1, "ligm");
	prepStmt.setInt(2, 30);
	prepStmt.setString(3, "WangXiangXinTian");
	prepStmt.addBatch();
	
	prepStmt.executeBatch();

	ResultSet rs = stmt.executeQuery("select *from people;");
	while(rs.next()){
	    System.out.println("name = " + rs.getString("name") + "," +
		    		"age = " + rs.getInt("age") + "," +
		    		"addr = " + rs.getString("addr"));
	}

	boolean hasResultSet = stmt.execute("select * from people");
	System.out.println("hasResultSet:" + hasResultSet );
	if(hasResultSet){
	    rs = stmt.getResultSet();
            while(rs.next()){
                System.out.println("name = " + rs.getString("name") + "," +
		    		"age = " + rs.getInt("age") + "," +
		    		"addr = " + rs.getString("addr"));
            }
	}

	conn.close();
    }

}