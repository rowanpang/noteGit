package pkg1;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import pkg2.PublicClass;

public class PublicClassMain {

    public static void main(String[] args) {
	
	PublicClass aPubClassObj = new PublicClass();
	aPubClassObj.pubClassPubMemString = "ok";
	PublicClass.pubClassPubStaticMemString = "ok";

        Class<?> clazz = null;
        Method method = null;

	try {
            clazz = Class.forName("pkg2.PublicClass");
	} catch (ClassNotFoundException e) {
	    e.printStackTrace();
	}

        try {
            method = clazz.getMethod("accessTest",String[].class); 	//public method will Ok.
        } catch (NoSuchMethodException e) {
            try { 							//private member will coming here
                method = clazz.getDeclaredMethod("accessTest", String[].class);
                method.setAccessible(true);
            } catch (NoSuchMethodException e1) {
                e1.printStackTrace();
            } catch (SecurityException e1) {
                e1.printStackTrace();
            }
        } catch (SecurityException e) {
            e.printStackTrace();
        }

        try {
            final Object[] argsRef= new Object[1];
            argsRef[0] = new String[] { "1","2"};
            try {
                final Object pkg2PubClassInvokerObj = clazz.newInstance();
                method.invoke(pkg2PubClassInvokerObj, argsRef);		//invoke method for specify obj.
                method.invoke(aPubClassObj, argsRef);
            } catch (InstantiationException e) {
                e.printStackTrace();
            }
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
	

    }

}
