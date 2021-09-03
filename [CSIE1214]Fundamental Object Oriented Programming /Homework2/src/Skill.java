
import tw.waterball.foop.hw2.provided.AI;

import java.util.ArrayList;
import java.util.List;

public class Skill {
    private Unit master;
    private Troop troop1;
    private Troop troop2;
    private ArrayList<Unit> targetList;
    private int HP_damage;
    private int MpCost;
    private String skillName;
    private int maxTargetNum;
    public Skill(){
        HP_damage = 0;
        MpCost = 0;
    }
    public Skill(Unit myMaster, String name, int HPD, int MPC, int myMaxNum, Troop myTroop1, Troop myTroop2){
        troop1 = myTroop1;
        troop2 = myTroop2;
        master = myMaster;
        skillName = name;
        HP_damage = HPD;
        MpCost = MPC;
        maxTargetNum = myMaxNum;
    }
    public Skill(int MPC){
        MpCost = MPC;
    }
    public void setTargetList(){
        ArrayList<Unit> candidate = new ArrayList<>();
        if(master.getTeamNum() == 1){
            for (Unit i: troop2.getList()) {
                if(i.getTeamNum() == master.getTeamNum() && i.getIdxInTroop() == master.getIdxInTroop()) continue;
                if(i.getHP() <= 0) continue;
                candidate.add(i);
            }
        }
        else{
            for (Unit i: troop1.getList()) {
                if(i.getTeamNum() == master.getTeamNum() && i.getIdxInTroop() == master.getIdxInTroop()) continue;
                if(i.getHP() <= 0) continue;
                candidate.add(i);
            }
        }

        if(master.getAI()){
            List<Integer> targetIdx;
            if(maxTargetNum < candidate.size()) {
                targetIdx = Main.ai.selectTarget(candidate.size(), maxTargetNum);
                targetList = new ArrayList<>();
                for(int i = 0; i < targetIdx.size(); ++i)
                    targetList.add(candidate.get(targetIdx.get(i)));
            }
            else targetList = candidate;
        }
        else{
            if(maxTargetNum < candidate.size()){
                System.out.printf("Select %d targets:", maxTargetNum);
                for(int i = 0; i < candidate.size() - 1; ++i)
                    System.out.printf(" (%d) [%d]%s", i, candidate.get(i).getTeamNum(), candidate.get(i).getName());
                int i = candidate.size() - 1;
                System.out.printf(" (%d) [%d]%s\n", i, candidate.get(i).getTeamNum(), candidate.get(i).getName());
                int[] command;
                String[] strCommand;
                String s = Inputs.in.nextLine();
                strCommand = s.split(", ");
                command = new int[strCommand.length];
                targetList = new ArrayList<>();
                for (int j = 0; j < strCommand.length; ++j) {
                    command[j] = Integer.parseInt(strCommand[j]);
                }
                for (int j = 0; j < strCommand.length; ++j)
                    targetList.add(candidate.get(command[j]));
            }
            else targetList = candidate;
        }
    }
    public void setTargetList2(ArrayList<Unit> myTargetList){
        targetList = myTargetList;
    }
    public void printAction(){
        System.out.printf("[%d]%s uses %s on ", master.getTeamNum(), master.getName(), skillName);
        for(int i = 0; i < targetList.size() - 1; ++i)
            System.out.printf("[%d]%s, ", targetList.get(i).getTeamNum(), targetList.get(i).getName());
        System.out.printf("[%d]%s.\n", targetList.get(targetList.size()-1).getTeamNum(), targetList.get(targetList.size()-1).getName());
    }
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        int newHP = target.getHP() - HP_damage - HPB;
        if(HP_damage + HPB >= 0)
            System.out.printf("[%d]%s causes %d damage to [%d]%s.\n", master.getTeamNum(), master.getName(), HP_damage + HPB, target.getTeamNum(), target.getName());
        target.setHP(newHP);
        if(newHP <= 0){
            for(int i = 0; i < target.deadActions.size(); ++i)
                target.deadActions.get(i).execute();
        }
    }
    public void decreaseMaterMP(){
        int masterNewMp = master.getMP() - MpCost;
        master.setMP(masterNewMp);
    }
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Skill a = new Skill();
        return a;
    }
    public int getMaxTargetNum(){
        return maxTargetNum;
    }
    public Unit getMaster(){
        return master;
    }
    public ArrayList<Unit> getTargetList(){
        return targetList;
    }
    public ArrayList<Unit> getTroop1(){
        return troop1.getList();
    }
    public ArrayList<Unit> getTroop2(){
        return troop2.getList();
    }
    public int getMpCost(){
        return MpCost;
    }
    public void setSkillName(String s){
        skillName = s;
    }
    @Override
    public String toString(){
        return skillName;
    }

}
