package stdio;

import java.io.IOException;

public class StdIO {
    public static void main(String[] args) {
	int tmp;
	char test;
	byte[] buf;
	final byte BUF_LEN = 30;
	ScannerTest st = new ScannerTest();
	st.test();

	System.out.println("please input: ");
	try {
	    while(true){
                tmp = System.in.read();
                System.out.println("input is: " + tmp);
                test = (char) tmp;
                System.out.println("input is: " + test + " end");
                
                if (tmp == 10){
                    System.out.println("break");
                    break;
                }
	    }
	    buf = new byte[BUF_LEN];
	    int count = System.in.read(buf);
	    System.out.println("length is: " + buf.length + ",read count: " + count);
	    for(int i=0; i < buf.length; i++){ 
		if( buf[i] == 10){
		    break;
		}
		System.out.println(buf[i] + " ");
	    }

	    String strTmp = new String(buf);
	    System.out.println(strTmp);

	} catch (IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
	
    }
}
