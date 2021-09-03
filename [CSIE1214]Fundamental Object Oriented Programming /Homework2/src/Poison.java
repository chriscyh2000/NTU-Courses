import java.util.ArrayList;

public class Poison extends Skill{
    public Poison(){
        super(80);
        super.setSkillName("Poison");
    }
    public Poison(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Poison", 0, 80, 1, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Poison p = new Poison(myMaster, myTroop1, myTroop2);
        return p;
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        ArrayList<Unit> checkList = new ArrayList<>();
        checkList.addAll(myTroop1.getList());
        checkList.addAll(myTroop2.getList());
        Poisoned st = new Poisoned(3, target, 30, myTroop1, myTroop2);
        target.setMyState(st);
        if(checkList.indexOf(target) < checkList.indexOf(super.getMaster()))
            st.setRemainRound(2);
    }
}
class Poisoned extends State{
    public Poisoned(){
        super();
    }
    public Poisoned(int myRemainRound, Unit myTarget, int myHPD, Troop myTroop1, Troop myTroop2){
        super(myRemainRound, "Poisoned", false, myHPD, myTarget, 0, myTroop1, myTroop2);
    }
}