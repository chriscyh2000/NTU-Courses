import java.util.ArrayList;

public class print_and_remove extends deadAction{
    public Troop myTroop;
    public print_and_remove(Unit m, Troop t){
        super(m);
        myTroop = t;
    }
    @Override
    public void execute(){
        System.out.printf("[%d]%s dies.\n", super.master.getTeamNum(), super.master.getName());
        myTroop.stillAlive--;
    }
}
