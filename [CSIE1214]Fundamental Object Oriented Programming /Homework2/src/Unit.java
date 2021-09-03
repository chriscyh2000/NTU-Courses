import tw.waterball.foop.hw2.provided.AI;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class Unit {
    private int HP;
    private int MP;
    private int STR;
    private int teamNum;
    private int idxInTroop;
    private String name;
    private State myState;
    public ArrayList<Skill> skillList;
    public ArrayList<deadAction> deadActions;
    private Troop troop;
    private boolean AI;
    public Unit(){
        MP = 100;
        HP = 100;
        STR = 0;
    }
    public Unit(int myMP, int myHP, int mySTR, int myTeamNum, int myIdxInTroop, String myName, boolean myAI, Troop myTroop){
        MP = myMP;
        HP = myHP;
        STR = mySTR;
        teamNum = myTeamNum;
        idxInTroop = myIdxInTroop;
        name = myName;
        AI = myAI;
        deadActions = new ArrayList<deadAction>();
        print_and_remove p = new print_and_remove(this, myTroop);
        deadActions.add(p);
        skillList = new ArrayList<>();
        troop = myTroop;
    }
    public int selectSkill(){
        if(AI){
            System.out.printf("Select an action: ");
            for (int i = 0; i < skillList.size() - 1; ++i)
                System.out.printf("(%d) %s ", i, skillList.get(i).toString());
            System.out.printf("(%d) %s\n", skillList.size() - 1, skillList.get(skillList.size() - 1).toString());
            List<Integer> a = new ArrayList<>();
            for(int i = 0; i < skillList.size(); ++i){
                if(skillList.get(i).getMpCost() <= MP)
                    a.add(i);
            }
            int chosen = Main.ai.selectAction(a);
            return chosen;
        }
        else{
            int chosen;
            do {
                System.out.print("Select an action: ");
                for (int i = 0; i < skillList.size() - 1; ++i)
                    System.out.printf("(%d) %s ", i, skillList.get(i).toString());
                System.out.printf("(%d) %s\n", skillList.size() - 1, skillList.get(skillList.size() - 1).toString());
                chosen = Integer.parseInt(Inputs.in.nextLine());
                if(skillList.get(chosen).getMpCost() > MP)
                    System.out.println("You can't perform the action: insufficient MP.");
                else return chosen;
            }while(skillList.get(chosen).getMpCost() > MP);
        }
        return 0;
    }
    public void printInformation(){
        System.out.printf("[%d]%s's turn (HP: %d, MP: %d, STR: %d, State: %s).\n", teamNum, name, HP, MP, STR, myState.toString());
    }
    public void setMP(int newMP){
        MP = newMP;
    }
    public void setHP(int newHP){
        HP = newHP;
    }
    public int getMP(){
        return MP;
    }
    public int getHP(){
        return HP;
    }
    public int getSTR(){
        return STR;
    }
    public State getMyState(){
        return myState;
    }
    public void setMyState(State st){
        myState = st;
    }
    public ArrayList<Skill> getSkillList(){
        return skillList;
    }
    public String getName(){
        return name;
    }
    public int getTeamNum(){
        return teamNum;
    }
    public int getIdxInTroop(){
        return idxInTroop;
    }
    public Troop getTroop(){return troop;}
    public boolean getAI(){
        return AI;
    }
}
