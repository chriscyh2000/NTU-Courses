import java.util.ArrayList;

public class Summon extends Skill{
    public Summon(){
        super(150);
        super.setSkillName("Summon");
    }
    public Summon(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Summon", 0, 150, 1, myTroop1, myTroop2);
        ArrayList<Unit> Master = new ArrayList<>();
        Master.add(myMaster);
        super.setTargetList2(Master);
    }
    @Override
    public void setTargetList(){}
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Summon s = new Summon(myMaster, myTroop1, myTroop2);
        return s;
    }
    @Override
    public void printAction(){
        System.out.printf("[%d]%s uses Summon.\n", super.getMaster().getTeamNum(), super.getMaster().getName());
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        Unit slime = new Unit(0, 100, 50, super.getMaster().getTeamNum(), -1, "Slime",
                true, super.getMaster().getTroop());
        BasicAttack b = new BasicAttack(slime, myTroop1, myTroop2);
        Normal n = new Normal(slime, myTroop1, myTroop2);
        slime.setMyState(n);
        slime.skillList.add(b);
        if(super.getMaster().getTeamNum() == 1) {
            myTroop1.getList().add(slime);
            myTroop1.stillAlive++;
        }
        else{
            myTroop2.getList().add(slime);
            myTroop2.stillAlive++;
        }
    }
}
