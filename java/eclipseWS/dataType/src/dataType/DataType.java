package dataType;

public class DataType {

    public static void main(String args[]){
	final double CM_PER_INCH = 2.54;		
	final double G=9.81; 				
	byte    b=011;					
	short   s=0x1E3;				
	int     i=-200000;   				
	long    l=0x10efL; 				
	char    c='c' ;         			
	float   f=0.114F;     				
	double  d=100.3E-2;   				
	boolean bf=false;     				
	boolean bt=true;      				

	System.out.println("CM_PER_INCH = "+CM_PER_INCH);
	System.out.println("G = "+G);
	System.out.println("b="+b);
	System.out.println("s="+s);
	System.out.println("i="+i);
	System.out.println("l="+l);
	System.out.println("c="+c);
	System.out.println("f="+f);
	System.out.println("d="+d);
	System.out.println("bf="+bf);
	System.out.println("bt="+bt);
	
	System.out.println("Integer.MAX_VALUE ="+ Integer.MAX_VALUE);		    
	System.out.println("Integer.MIN_VALUE ="+ Integer.MIN_VALUE);		    
	System.out.println("Long.MAX_VALUE ="+ Long.MAX_VALUE);			    
	System.out.println("Long.MIN_VALUE ="+Long.MIN_VALUE);		    	    
	System.out.println("Float.MAX_VALUE ="+ Float.MAX_VALUE);		    
	System.out.println("Float.MIN_VALUE ="+ Float.MIN_VALUE); 		    
	System.out.println("Double.MAX_VALUE ="+ Double.MAX_VALUE); 		    
	System.out.println("Double.MIN_VALUE ="+ Double.MIN_VALUE);		    
	System.out.println("Float.POSITIVE_INFINITY ="+ Float.POSITIVE_INFINITY);   
	System.out.println("Float.NEGATIVE_INFINITY ="+ Float.NEGATIVE_INFINITY);   
	System.out.println("Double.POSITIVE_INFINITY =" + Double.POSITIVE_INFINITY);
	System.out.println("Double.NEGATIVE_INFINITY =" + Double.NEGATIVE_INFINITY);
	System.out.println("Float.NaN ="+ Float.NaN);				    
	System.out.println("Double.NaN ="+ Double.NaN);				    
    }
}
