import tw.waterball.foop.hw2.provided.AI;

import java.util.ArrayList;
import java.util.List;

public class Cheerup extends Skill{
    public Cheerup(){
        super(100);
        super.setSkillName("Cheerup");
    }
    public Cheerup(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Cheerup", 0, 100, 3, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Cheerup c = new Cheerup(myMaster, myTroop1, myTroop2);
        return c;
    }
    @Override
    public void setTargetList(){
        ArrayList<Unit> candidate = new ArrayList<>();
        if(super.getMaster().getTeamNum() == 1){
            for (Unit i: super.getTroop1()) {
                if(i.getIdxInTroop() == super.getMaster().getIdxInTroop()) continue;
                if(i.getHP() <= 0) continue;
                candidate.add(i);
            }
        }
        else{
            for (Unit i: super.getTroop2()) {
                if(i.getIdxInTroop() == super.getMaster().getIdxInTroop()) continue;
                if(i.getHP() <= 0) continue;
                candidate.add(i);
            }
        }

        if(super.getMaster().getAI()){
            List<Integer> targetIdx;
            if(super.getMaxTargetNum() < candidate.size()) {
                targetIdx = Main.ai.selectTarget(candidate.size(), super.getMaxTargetNum());
                ArrayList<Unit> targetList = new ArrayList<>();
                for(int i = 0; i < targetIdx.size(); ++i)
                    targetList.add(candidate.get(targetIdx.get(i)));
                super.setTargetList2(targetList);
            }
            else super.setTargetList2(candidate);
        }
        else{
            if(super.getMaxTargetNum() < candidate.size()){
                System.out.printf("Select %d targets:", super.getMaxTargetNum());
                for(int i = 0; i < candidate.size() - 1; ++i)
                    System.out.printf(" (%d) [%d]%s", i, candidate.get(i).getTeamNum(), candidate.get(i).getName());
                int i = candidate.size() - 1;
                System.out.printf(" (%d) [%d]%s\n", i, candidate.get(i).getTeamNum(), candidate.get(i).getName());
                int[] command;
                String[] strCommand;
                strCommand = Inputs.in.nextLine().split(", ");
                command = new int[strCommand.length];
                ArrayList<Unit> targetList = new ArrayList<>();
                for (int j = 0; j < strCommand.length; ++j)
                    command[j] = Integer.parseInt(strCommand[j]);
                for (int j = 0; j < strCommand.length; ++j)
                    targetList.add(candidate.get(command[j]));
                super.setTargetList2(targetList);
            }
            else super.setTargetList2(candidate);
        }
    }
    @Override
    public void perform(Unit target, int HPB, Troop myTroop1, Troop myTroop2){
        ArrayList<Unit> checkList = new ArrayList<>();
        checkList.addAll(myTroop1.getList());
        checkList.addAll(myTroop2.getList());
        Cheerup_st c = new Cheerup_st(3, 50, target, myTroop1, myTroop2);
        target.setMyState(c);
        if(checkList.indexOf(target) < checkList.indexOf(super.getMaster())){
            target.getMyState().effect();
            target.getMyState().decreaseRemainRound();
        }
    }
}
class Cheerup_st extends State{
    public Cheerup_st(){
        super();
    }
    public Cheerup_st(int myRemainRound, int myHPB, Unit myTarget, Troop myTroop1, Troop myTroop2){
        super(myRemainRound, "Cheerup", false, 0, myTarget, myHPB, myTroop1, myTroop2);
    }
    @Override
    public void effect(){}
}