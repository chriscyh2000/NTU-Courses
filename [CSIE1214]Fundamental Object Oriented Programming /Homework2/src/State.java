import java.util.ArrayList;
public class State {
    private int remainRound;
    private int HP_decrease;
    public boolean skip;
    private int HP_damageBonus;
    private Unit target;
    private String stateName;
    private Troop troop1;
    private Troop troop2;
    public State(){
        skip = false;
        HP_decrease = 0;
        HP_damageBonus = 0;
        remainRound = 3;
    }
    public State(int myRemainRound, String myName, boolean mySkip, int myHPD, Unit myTarget, int myHPB, Troop myTroop1, Troop myTroop2){
        remainRound = myRemainRound;
        HP_decrease = myHPD;
        skip = mySkip;
        stateName = myName;
        target = myTarget;
        HP_damageBonus = myHPB;
        troop1 = myTroop1;
        troop2 = myTroop2;
    }
    @Override
    public String toString() {
        return stateName;
    }
    public void effect(){
        int newHP = target.getHP() - HP_decrease;
        target.setHP(newHP);
        if(newHP <= 0){
            for(int i = 0; i < target.deadActions.size(); ++i)
                target.deadActions.get(i).execute();
        }
    }
    public void decreaseRemainRound(){
        --remainRound;
        if(remainRound == 0) {
            Normal normalState = new Normal(target, troop1, troop2);
            target.setMyState(normalState);
        }
    }
    public int getHP_damageBonus(){
        return HP_damageBonus;
    }
    public Unit getTarget(){
        return target;
    }
    public ArrayList<Unit> getTroop1(){
        return troop1.getList();
    }
    public ArrayList<Unit> getTroop2(){
        return troop2.getList();
    }

    public void setRemainRound(int remainRound) {
        this.remainRound = remainRound;
    }
}
