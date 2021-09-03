import java.util.ArrayList;

public class Petrochemical extends Skill{
    public Petrochemical(){
        super(100);
        super.setSkillName("Petrochemical");
    }
    public Petrochemical(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Petrochemical", 0, 100, 1, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Petrochemical p = new Petrochemical(myMaster, myTroop1, myTroop2);
        return p;
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        ArrayList<Unit> checkList = new ArrayList<>();
        checkList.addAll(myTroop1.getList());
        checkList.addAll(myTroop2.getList());
        Petrochemicalst st = new Petrochemicalst(3, target, myTroop1, myTroop2);
        target.setMyState(st);
        if(checkList.indexOf(target) < checkList.indexOf(super.getMaster()))
            st.setRemainRound(2);
    }
}
class Petrochemicalst extends State{
    public Petrochemicalst(){
        super();
    }
    public Petrochemicalst(int myRemainRound, Unit myTarget, Troop myTroop1, Troop myTroop2){
        super(myRemainRound, "Petrochemical", true, 0, myTarget, 0, myTroop1, myTroop2);
    }
    @Override
    public void effect(){}
}
