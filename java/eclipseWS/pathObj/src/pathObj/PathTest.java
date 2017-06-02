package pathObj;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;

public class PathTest {

    public static void main(String[] args) {
	System.out.println(System.getProperty("user.dir"));
	System.out.println(System.getProperty("user.home"));
	System.out.println(System.getProperty("java.home"));
	System.out.println(System.getProperty("java.io.tmpdir"));
	System.out.println(System.getProperty("java.class.path"));
	System.out.println(System.getProperty("file.separator"));
	
	PathTest pt = new PathTest();
        pt.getCurPath();

    }
    
    private String getCurPath() {
	
	File dir = new File("a.txt");
        try {
	    System.out.println(dir.getCanonicalPath());
	} catch (IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
        System.out.println(dir.getAbsolutePath());

        System.out.println(dir.getPath());

	return "ok";
    }

}
