
package serial;

import java.io.Serializable;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

@SuppressWarnings("serial")
class myPerson implements Serializable {
    private String name;
    private int age;
    
    myPerson(String n,int age){
	this.name = n;
	this.age = age;
    }
    
    public String getName(){
	return this.name;
    }
    
    public int getAge(){
	return this.age;
    }

    public int getAge_2(){
	return this.age;
    }
    
    public int setName(String n){
	this.name = n;
	return 0;
    }

    public int setAge(int n){
	this.age = n;
	return 0;
    }
    
    public String toString(){
	return "[name:" + this.name + " age:" + this.age + "]";
    }
}

public class SimpleSerial {

    public static void main(String[] args) {
	final String fileName = "jSerialTest.bin";

	myPerson rowan = new myPerson("rowanPang",30);
	System.out.println(rowan);
	
	myPerson li = new myPerson("li",22);
        FileOutputStream osFile = null;
        FileInputStream inFile = null;

	try {
	    osFile = new FileOutputStream(fileName);
	} catch (FileNotFoundException e1) {
	    // TODO Auto-generated catch block
	    e1.printStackTrace();
	}
	
	try {
	    ObjectOutputStream os = new ObjectOutputStream(osFile);
	    os.writeObject(li);
	} catch (IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
	
        try {
	    inFile = new FileInputStream(fileName);
	} catch (FileNotFoundException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
        
        ObjectInputStream is = null;
        try {
            is = new ObjectInputStream(inFile);
	} catch (IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
        
        myPerson liObj = null;
        try {
	    liObj = (myPerson) is.readObject();
	} catch (ClassNotFoundException | IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
        
        System.out.println(liObj);
    }

}
