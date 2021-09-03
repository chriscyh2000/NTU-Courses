import java.util.ArrayList;

public class Troop {
    private ArrayList<Unit> list;
    private int idx;
    public int stillAlive;
    public Troop(){
        list = new ArrayList<Unit>();
        stillAlive = 0;
    }
    public Troop(int myIdx){
        list = new ArrayList<Unit>();
        idx = myIdx;
        stillAlive = 0;
    }
    public ArrayList<Unit> getList(){
        return list;
    }
    public int getIdx(){
        return idx;
    }
}
