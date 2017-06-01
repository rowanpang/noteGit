package param;

public class ParamTest {
    int numArray[] = {2, 3, 4, 5, 6};
    public static void main(String args[]) {
	ParamTest pt = new ParamTest();
	pt.basic();
	pt.paramRef();
	pt.stringParam();
    }

    private void modify(int a[]) {
	for (int i = 0; i < a.length; i++) {
	    a[i] *= 2;
	}
    }
    
    private void modify(int a, int b){
	a += 2;
	b += 3;
    }
    
    private void modify(String str){
	str = "string after modify";
    }

    private void paint(int a[], String prompt) {
	String temp = "";
	for (int i = 0; i < a.length; i++) {
	    temp += a[i] + " ";
	}

	System.out.printf("%15s%s\n",prompt,temp);
    }

    private void paramRef() {
	this.paint(numArray, "before modify: ");
	this.modify(numArray);
	this.paint(numArray, "after modify: ");
    }
    
    private void basic(){
	int a=0;
	int b=2;
	System.out.println("before modify: " + "a=" + a + ",b=" + b);
	this.modify(a,b);
	System.out.println("after modify: a=" + a + ",b=" + b);
    }
    
    private void stringParam(){
	String tmp = "this is a string";
	System.out.println("before modify: " + tmp);
	this.modify(tmp);
	System.out.println("after modify: " + tmp);
    }

}
