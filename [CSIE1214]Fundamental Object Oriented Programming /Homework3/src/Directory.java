import java.util.ArrayList;
import java.util.Collections;

public class Directory extends Element {
    private ArrayList<Element> ElementList;
    private String path;
    private Directory parentDir;
    public Directory(){
        ElementList = new ArrayList<>();
    }
    public Directory(String myPath, String myName, Directory myParent){
        super(myName);
        ElementList = new ArrayList<>();
        path = myPath;
        parentDir = myParent;
    }
    public Element getElement(String targetName){
        if(targetName.equals("..")) return parentDir;
        int size = ElementList.size();
        Element target;
        int i = 0;
        for(; i < size; ++i){
            target = ElementList.get(i);
            if(target.getName().equals(targetName))
                return target;
        }
        if(i == size) System.out.println("Illegal command.");
        return this;
    }
    public void insertNewElement(String newElementName){
        int i = ElementList.size() - 2;
        while(i >= 0 && newElementName.compareTo(ElementList.get(i).getName()) < 0){
            Collections.swap(ElementList, i, i + 1);
            --i;
        }
    }
    @Override
    public Element createNewElement(String info1, String info2, Element related){
        Directory parent = (Directory)related;
        Element newElement;
        if(parent.getName().equals("/"))
            newElement = new Directory("/" + info1, info1,parent);
        else newElement = new Directory(parent.getPath() + "/" + info1, info1, parent);
        return newElement;
    }
    public void addNewElement(String info1, String info2, Element toCreat){
        int size = ElementList.size();
        for(int i = 0; i < size; ++i){
            if(ElementList.get(i).getName().equals(info1)){
                System.out.println("Illegal command.");
                return;
            }
        }
        Element newElement = toCreat.createNewElement(info1, info2, this);
        ElementList.add(newElement);
        insertNewElement(info1);
    }
    public void removeElement(String targetName){
        int size = ElementList.size();
        for(int i = 0; i < size; ++i){
            if(ElementList.get(i).getName().equals(targetName)){
                ElementList.remove(i);
                return;
            }
        }
        System.out.println("Illegal command.");
    }
    public String getPath() {
        return path;
    }
    public void setParentDir(Directory myParentDir){
        parentDir = myParentDir;
    }
    public void listAllElement(){
        int size = ElementList.size();
        for(int i = 0; i < size; ++i)
            System.out.println(ElementList.get(i).getName());
    }
    public void search(String keyword){
        int size = ElementList.size();
        if(super.getName().contains(keyword))
            System.out.println(super.getName());
        Directory cur;
        for(int i = 0; i < size; ++i){
            if(ElementList.get(i).toCheck(this) != this){
                cur = (Directory) ElementList.get(i);
                cur.search(keyword);
            }
            else{
                if(ElementList.get(i).getName().contains(keyword))
                    System.out.println(ElementList.get(i).getName());
            }
        }
    }
    public ArrayList<Element> getElementList(){
        return ElementList;
    }
    @Override
    public Element toCheck(Element curDir){
        return this;
    }
}