package pkg2;

class NonPubClass {
    public String name;
    
    public int NonPubClassPubStaticFunc(String[] args){
	System.out.println("hello world from: " + this.getMethodName());
	return 0;
    }
    
    private String getMethodName(){
	StackTraceElement[] stacktrace = Thread.currentThread().getStackTrace();
	StackTraceElement e = stacktrace[2];

	String name = e.getMethodName();
	
	return name;
    }
}
