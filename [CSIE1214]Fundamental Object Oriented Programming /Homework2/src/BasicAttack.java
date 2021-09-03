import java.util.ArrayList;

public class BasicAttack extends Skill{
    public BasicAttack(){
        super(0);
    }
    public BasicAttack(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Basic Attack", myMaster.getSTR(), 0, 1, myTroop1, myTroop2);
    }
    @Override
    public void printAction(){
        Unit target = super.getTargetList().get(0);
        System.out.printf("[%d]%s attacks [%d]%s.\n", super.getMaster().getTeamNum(), super.getMaster().getName(),target.getTeamNum(), target.getName());
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        BasicAttack b = new BasicAttack(myMaster, myTroop1, myTroop2);
        return b;
    }
}
