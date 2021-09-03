import java.util.ArrayList;

public class SelfHealing extends Skill{
    public SelfHealing(){
        super(50);
        super.setSkillName("SelfHealing");
    }
    public SelfHealing(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "SelfHealing", 0, 50, 1, myTroop1, myTroop2);
        ArrayList<Unit> Master = new ArrayList<>();
        Master.add(myMaster);
        super.setTargetList2(Master);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        SelfHealing s = new SelfHealing(myMaster, myTroop1, myTroop2);
        return s;
    }
    @Override
    public void setTargetList(){}
    @Override
    public void printAction(){
        System.out.printf("[%d]%s uses SelfHealing.\n", super.getMaster().getTeamNum(), super.getMaster().getName());
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        int newHP = target.getHP() + 150;
        target.setHP(newHP);
    }

}