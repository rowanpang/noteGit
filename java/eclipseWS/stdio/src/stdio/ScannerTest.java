package stdio;

import java.util.Scanner;

public class ScannerTest {
    Scanner s;
    
    public ScannerTest(){
	this.s = new Scanner(System.in);
    }
    
    public void test(){
	while(true){
            System.out.println("----------scanner new start please input:");
            String inData = this.s.nextLine();
            System.out.println("input is: " + inData);
            if (inData.startsWith("q")){
                System.out.println("-------------scanner test break");
        	break;
            }

            System.out.println("enter a double:");
            double df = this.s.nextDouble();
            System.out.println("double is: " + df);

            System.out.println("enter a int:");
            int i = this.s.nextInt();
            System.out.println("int is: " + i);
            
            this.s.nextLine();
	}
    }

}
