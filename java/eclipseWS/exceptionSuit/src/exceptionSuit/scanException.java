package exceptionSuit;

import java.util.InputMismatchException;
import java.util.Scanner;

public class scanException {

    public static void main(String[] args) {
	int inData = 0;
	Scanner s;

	while (true) {
	    System.out.print("input integer:");
            s = new Scanner(System.in);

	    try {
                inData = s.nextInt();
	    } catch(InputMismatchException e) {
		System.out.println("input is no legal,please input integer!!");
		continue;
	    }

	    System.out.println("input is: " + inData);

	    if (inData == 10) {
		break;
	    }
	}
	
	System.out.println("----------finish-------");
	s.close();
    }

}