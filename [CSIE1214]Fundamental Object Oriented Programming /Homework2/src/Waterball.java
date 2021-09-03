import java.util.ArrayList;

public class Waterball extends Skill{
    public Waterball(){
        super(50);
        super.setSkillName("Waterball");
    }
    public Waterball(Unit myMaster, Troop myTroop1, Troop myTroop2){
        super(myMaster, "Waterball", 120, 50, 1, myTroop1, myTroop2);
    }
    @Override
    public Skill createSkill(Unit myMaster, Troop myTroop1, Troop myTroop2){
        Waterball w = new Waterball(myMaster, myTroop1, myTroop2);
        return w;
    }
}