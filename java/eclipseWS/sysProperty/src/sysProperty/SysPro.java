package sysProperty;

import java.util.Enumeration;
import java.util.Iterator;
import java.util.TreeSet;
import java.util.function.Consumer;

class SysPro { 					//non public is still OK

    public static void main(String[] args) {
	String proName = null;
	TreeSet<String> sortedNames = new TreeSet<>();
	
	@SuppressWarnings("unchecked")
	Enumeration<String> proNames = (Enumeration<String>) System.getProperties().propertyNames();

	//use treeset to sort keys;
	while (proNames.hasMoreElements()){
	    proName = proNames.nextElement();
	    sortedNames.add(proName);
	}

	//query keys --method.1
/*	for (String sortName: sortedNames){
            System.out.println(sortName + "=" + System.getProperty(sortName));
	}
*/	
	//query keys --method.2
/*	Iterator<String> treeIt = sortedNames.iterator();
	while (treeIt.hasNext()){
	    proName = treeIt.next();
            System.out.println(proName + "=" + System.getProperty(proName));
	}
*/	
	Consumer<String> itAction = SysPro::msgOut;
	sortedNames.forEach(itAction);

//	Consumer<String> itAction2 = (pro) -> System.out.println(pro + "=" + System.getProperty(pro));
//	sortedNames.forEach(itAction2);
	
/*	System.out.println(System.getProperties().propertyNames().toString());
	System.getProperties().list(System.out);
	System.out.println(System.getProperties());
*/    
    }
    
    private static void msgOut(String pro){
	System.out.println(pro + "=" + System.getProperty(pro));
    }

}
