public class File extends Element{
    private String content;
    public File(){
        super();
        content = "";
    }
    public File(String myName, String myContent){
        super(myName);
        content = myContent;
    }
    public String getContent(){
        return content;
    }
    @Override
    public Element createNewElement(String info1, String info2, Element related){
        Element newElement = new File(info1, info2);
        return newElement;
    }
}
