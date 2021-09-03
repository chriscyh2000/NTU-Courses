import java.util.ArrayList;

public class Link extends File{
    private Directory linkedTo;
    public Link(){}
    public Link(String myContent, String myName, Directory myTarget){
        super(myName, myContent);
        linkedTo = myTarget;
    }
    @Override
    public Element createNewElement(String info1, String info2, Element related){
        Directory curRelated = (Directory) related;
        ArrayList<Element> curList = curRelated.getElementList();
        int size = curList.size();
        Element cur = new Element();
        int i = 0;
        for(; i < size; ++i){
            cur = curList.get(i);
            if(cur.getName().equals(info2) && cur.toCheck(related) == cur)
                break;
        }
        Link newLink;
        if(i == size){
            newLink = new Link(info2, info1, (Directory) related);
            System.out.println("Illegal command.");
            return newLink;
        }
        newLink = new Link(info2, info1, (Directory) cur);
        return newLink;
    }
    @Override
    public Element toCheck(Element curDir){
        if(curDir == null) return null;
        ArrayList<Element> List = ((Directory)curDir).getElementList();
        int size = List.size();
        int i = 0;
        for(; i < size; ++i){
            if(List.get(i).getName().equals(super.getContent())){
                if(List.get(i).toCheck(curDir) == curDir)
                    return curDir;
                else{
                    linkedTo = (Directory) List.get(i);
                    return linkedTo;
                }
            }
        }
        return curDir;
    }
    @Override
    public String getContent(){
        return linkedTo.getPath();
    }
}