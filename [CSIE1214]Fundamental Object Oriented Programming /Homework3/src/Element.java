import java.util.ArrayList;
public class Element {
    private String name;
    public Element(){
        name = "";
    }
    public Element(String myName){
        name = myName;
    }
    public String getName(){
        return name;
    }

    public Element createNewElement(String info1, String info2, Element related){
        Element newElement = new Element();
        return newElement;
    }
    public Element toCheck(Element curDir){
        return curDir;
    }
}
