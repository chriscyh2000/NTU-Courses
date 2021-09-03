import tw.waterball.foop.hw2.provided.OnePunch;
import tw.waterball.foop.hw2.provided.Target;

import java.util.ArrayList;

public class OnePunch_skill extends Skill{
    public OnePunch_skill(){
        super(180);
        super.setSkillName("OnePunch");
    }
    public OnePunch_skill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "OnePunch", 0, 180, 1, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        OnePunch_skill o = new OnePunch_skill(myMaster, myTroop1, myTroop2);
        return o;
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        toGetOnePunch onepunch = new toGetOnePunch(super.getMaster(), super.getMaster().getMyState().getHP_damageBonus(), target);
        OnePunch toExecute = new OnePunch();
        toExecute.perform(onepunch);
    }
}
class toGetOnePunch implements Target{
    private Unit p;
    private int p_HPB;
    private Unit me;
    public toGetOnePunch(Unit pp, int pp_HPB, Unit I){
        p = pp;
        p_HPB = pp_HPB;
        me = I;
    }
    @Override
    public void takeOnePunchDamage(int damage){
        int newHP = me.getHP() - damage - p_HPB;
        System.out.printf("[%d]%s causes %d damage to [%d]%s.\n", p.getTeamNum(), p.getName(), damage + p_HPB, me.getTeamNum(), me.getName());
        me.setHP(newHP);
        if(newHP <= 0){
            for(int i = 0; i < me.deadActions.size(); ++i)
                me.deadActions.get(i).execute();
        }
    }
}
