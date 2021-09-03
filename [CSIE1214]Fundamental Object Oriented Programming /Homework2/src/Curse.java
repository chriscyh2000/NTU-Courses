import java.util.ArrayList;

public class Curse extends Skill{
    public Curse(){
        super(100);
        super.setSkillName("Curse");
    }
    public Curse(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Curse", 0, 100, 1, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Curse c = new Curse(myMaster, myTroop1, myTroop2);
        return c;
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        boolean exist = false;
        int i;
        for(i = 0; i < target.deadActions.size(); ++i){
            if(target.deadActions.get(i).toString().equals("CurseAction")){
                exist = true;
                break;
            }
        }
        if(exist){
            CurseAction c;
            c = (CurseAction) target.deadActions.get(i);
            if(!c.toIncreaseHP.contains(super.getMaster()))
                c.toIncreaseHP.add(super.getMaster());
        }
        else{
            CurseAction c = new CurseAction(target, super.getMaster());
            target.deadActions.add(c);
        }
    }
}
class CurseAction extends deadAction{
    public ArrayList<Unit> toIncreaseHP;
    public CurseAction(Unit m, Unit i){
        super(m);
        toIncreaseHP = new ArrayList<>();
        toIncreaseHP.add(i);
    }
    @Override
    public void execute(){
        for(int i = 0; i < toIncreaseHP.size(); ++i){
            if(toIncreaseHP.get(i).getHP() <= 0) continue;
            int newHP = toIncreaseHP.get(i).getHP() + super.master.getMP();
            toIncreaseHP.get(i).setHP(newHP);
        }
    }
    @Override
    public String toString(){return "CurseAction"; }

}