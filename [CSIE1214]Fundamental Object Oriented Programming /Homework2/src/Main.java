import tw.waterball.foop.hw2.provided.AI;

import java.util.ArrayList;

/**
 * The RPG game will be started from the Main class.
 * Haha
 */
public class Main {
    public static AI ai = new AI();
    public static void main(String[] args) {
        ArrayList<Skill> testSkillList = new ArrayList<>();
        BasicAttack ba = new BasicAttack();
        Waterball test1 = new Waterball();
        testSkillList.add(test1);
        Fireball test2 = new Fireball();
        testSkillList.add(test2);
        SelfHealing test3 = new SelfHealing();
        testSkillList.add(test3);
        Petrochemical test4 = new Petrochemical();
        testSkillList.add(test4);
        Poison test5 = new Poison();
        testSkillList.add(test5);
        Summon test6 = new Summon();
        testSkillList.add(test6);
        SelfExplosion test7 = new SelfExplosion();
        testSkillList.add(test7);
        Cheerup test8 = new Cheerup();
        testSkillList.add(test8);
        Curse test9 = new Curse();
        testSkillList.add(test9);
        OnePunch_skill test10 = new OnePunch_skill();
        testSkillList.add(test10);
        //If you want to add a new skill, just add a line here: <Skill class> test11 = new <Skill class>();
        //And add it into the testSkillList: testSkillList.add(test11);
        //If you want a skill be removed, just delete the initialization of the skill here and delete the skill of the class.
        Troop troop1 = new Troop(1);
        Troop troop2 = new Troop(2);
        Inputs.in.nextLine();
        String s = Inputs.in.nextLine();
        int num = 0;
        Skill temp;
        while(!s.equals("#END-TROOP-1")){
            String[] information;
            information = s.split(" ");
            int hp = Integer.parseInt(information[1]);
            int mp = Integer.parseInt(information[2]);
            int str = Integer.parseInt(information[3]);
            boolean ai;
            if(num == 0) ai = false;
            else ai = true;
            Unit newUnit = new Unit(mp, hp, str, 1, num, information[0], ai, troop1);
            Normal n = new Normal(newUnit, troop1, troop2);
            newUnit.setMyState(n);
            troop1.getList().add(newUnit);
            troop1.stillAlive++;
            temp = ba.createSkill(newUnit, troop1, troop2);
            newUnit.skillList.add(temp);
            for(int i = 4; i < information.length; ++i){
                for(int j = 0; j < testSkillList.size(); ++j){
                    if(information[i].equals(testSkillList.get(j).toString())){
                        temp = testSkillList.get(j).createSkill(newUnit, troop1, troop2);
                        newUnit.skillList.add(temp);
                        break;
                    }
                }
            }
            ++num;
            s = Inputs.in.nextLine();
        }
        Inputs.in.nextLine();
        s = Inputs.in.nextLine();
        num = 0;
        while(!s.equals("#END-TROOP-2")){
            String[] information;
            information = s.split(" ");
            int hp = Integer.parseInt(information[1]);
            int mp = Integer.parseInt(information[2]);
            int str = Integer.parseInt(information[3]);
            Unit newUnit = new Unit(mp, hp, str, 2, num, information[0], true, troop2);
            Normal n = new Normal(newUnit, troop1, troop2);
            newUnit.setMyState(n);
            troop2.getList().add(newUnit);
            troop2.stillAlive++;
            temp = ba.createSkill(newUnit, troop1, troop2);
            newUnit.skillList.add(temp);
            for(int i = 4; i < information.length; ++i){
                for(int j = 0; j < testSkillList.size(); ++j){
                    if(information[i].equals(testSkillList.get(j).toString())){
                        temp = testSkillList.get(j).createSkill(newUnit, troop1, troop2);
                        newUnit.skillList.add(temp);
                        break;
                    }
                }
            }
            ++num;
            s = Inputs.in.nextLine();
        }
        while(troop2.stillAlive > 0 && troop1.getList().get(0).getHP() > 0){
            for(int i = 0; i < troop1.getList().size(); ++i){
                if(troop1.getList().get(0).getHP() <= 0 || troop2.stillAlive <= 0) break;
                Unit cur = troop1.getList().get(i);
                if(cur.getHP() <= 0) continue;
                cur.printInformation();
                cur.getMyState().effect();
                if(cur.getHP() <= 0 || cur.getMyState().skip) {
                    cur.getMyState().decreaseRemainRound();
                    continue;
                }
                Skill curSkill = cur.skillList.get(cur.selectSkill());
                curSkill.setTargetList();
                curSkill.printAction();
                for(int j = 0; j < curSkill.getTargetList().size(); ++j){
                    curSkill.perform(curSkill.getTargetList().get(j), cur.getMyState().getHP_damageBonus(), troop1, troop2);
                }
                curSkill.decreaseMaterMP();
                if(cur.getHP() <= 0){
                    for(int j = 0; j < cur.deadActions.size(); ++j)
                        cur.deadActions.get(j).execute();
                }
                cur.getMyState().decreaseRemainRound();
            }
            for(int i = 0; i < troop2.getList().size(); ++i){
                if(troop1.getList().get(0).getHP() <= 0 || troop2.stillAlive <= 0) break;
                Unit cur = troop2.getList().get(i);
                if(cur.getHP() <= 0) continue;
                cur.printInformation();
                cur.getMyState().effect();
                if(cur.getHP() <= 0 || cur.getMyState().skip) {
                    cur.getMyState().decreaseRemainRound();
                    continue;
                }
                Skill curSkill = cur.skillList.get(cur.selectSkill());
                curSkill.setTargetList();
                curSkill.printAction();
                for(int j = 0; j < curSkill.getTargetList().size(); ++j){
                    curSkill.perform(curSkill.getTargetList().get(j), cur.getMyState().getHP_damageBonus(), troop1, troop2);
                }
                curSkill.decreaseMaterMP();
                if(cur.getHP() <= 0){
                    for(int j = 0; j < cur.deadActions.size(); ++j)
                        cur.deadActions.get(j).execute();
                }
                cur.getMyState().decreaseRemainRound();
            }
        }
        if(troop1.getList().get(0).getHP() <= 0) System.out.println("You lose.");
        else System.out.println("You win.");
    }
}