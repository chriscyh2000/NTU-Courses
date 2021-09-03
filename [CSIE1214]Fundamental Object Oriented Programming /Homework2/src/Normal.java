import java.util.ArrayList;

public class Normal extends State{
    public Normal(){
        super();
    }
    public Normal(Unit myTarget , Troop myTroop1, Troop myTroop2){
        super(Integer.MAX_VALUE, "Normal", false, 0, myTarget, 0, myTroop1, myTroop2);
    }
    @Override
    public void effect(){}
}
