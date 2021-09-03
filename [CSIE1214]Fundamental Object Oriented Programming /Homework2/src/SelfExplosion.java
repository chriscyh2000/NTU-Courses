import java.util.ArrayList;

public class SelfExplosion extends Skill{
    public SelfExplosion(){
        super(200);
        super.setSkillName("SelfExplosion");
    }
    public SelfExplosion(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "SelfExplosion", 150, 200, 0, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        SelfExplosion s = new SelfExplosion(myMaster, myTroop1, myTroop2);
        return s;
    }
    @Override
    public void setTargetList(){
        ArrayList<Unit> candidate = new ArrayList<>();
        for (Unit i: super.getTroop1()) {
            if(i.getTeamNum() == super.getMaster().getTeamNum() && i.getIdxInTroop() == super.getMaster().getIdxInTroop()) continue;
            if(i.getHP() <= 0) continue;
            candidate.add(i);
        }
        for (Unit i: super.getTroop2()) {
            if(i.getTeamNum() == super.getMaster().getTeamNum() && i.getIdxInTroop() == super.getMaster().getIdxInTroop()) continue;
            if(i.getHP() <= 0) continue;
            candidate.add(i);
        }
        super.setTargetList2(candidate);
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        super.perform(target, HPB, myTroop1, myTroop2);
        super.getMaster().setHP(0);
    }
}
