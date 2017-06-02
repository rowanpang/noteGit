package pkg2;

public class PublicClass {
    public static String pubClassPubStaticMemString;
    public String pubClassPubMemString;

    static String pubClassStaticMemString;
    String pubClassMemString;
    
    
    public int accessTest(String[] args){
//    private int accessTest(String[] args){
	
	NonPubClass nonPubClassObj = new NonPubClass();
	nonPubClassObj.name = "sds";
	nonPubClassObj.NonPubClassPubStaticFunc(new String[] {"asd"});
	
	return 0;
    }
}
