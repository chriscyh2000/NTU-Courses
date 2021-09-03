import java.util.ArrayList;

public class Fireball extends Skill{
    public Fireball(){
        super(50);
        super.setSkillName("Fireball");
    }
    public Fireball(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Fireball", 50, 50, 0, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Fireball f = new Fireball(myMaster, myTroop1, myTroop2);
        return f;
    }
    @Override
    public void setTargetList(){
        ArrayList<Unit> candidate;
        if(super.getMaster().getTeamNum() == 1){
            candidate = new ArrayList<>();
            for(Unit i : super.getTroop2()){
                if(i.getHP() <= 0) continue;
                candidate.add(i);
            }
        }
        else {
            candidate = new ArrayList<>();
            for(Unit i : super.getTroop1()){
                if(i.getHP() <= 0) continue;
                candidate.add(i);
            }
        }
        super.setTargetList2(candidate);
    }
}
