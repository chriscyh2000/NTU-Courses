# Design-Report(HW2).md
## NTU CSIE FOOP Spring 2021  <br><font size = 3px>B08902149  資工二 徐晨祐</font>
### Project Structure:
- **Inputs : Contorl all input stream.**
- **Unit : Record all the imformation of a certain unit.**
    - I save all the basic information of a unit such as `HP`, `MP`, `Name`.
    - I use a boolean `AI` to recognize if the unit is AI-controlled.
    - I used a `ArrayList<Skill>` and `ArrayList<deadAction>` to record all the skill that a unit have and what actions the unit have to do when it dies.
- **Troop : To record the units in a certain troop and the basic information of the troop.**
- **Skill : Set up all the common information and methods among all the skills.**
    - Use`setTargetList()` to set the target that this skill act on.
    - Use `printAction()` to print what the skill have done.
    - Use `perform()` for the skill to perform.
- **State : Set up all the common information and methods among all the states.**
    - Use `effect()` to show how this state effect the unit which it belongs to.
    - Use `decreaseRemainRound()` to decrease the remaining round of the state.
    - If the state is related to a certain skill and only exist when someone call the skill, then I wrap the state class into the java file of the skill. Because if the skill need to be deleted, the state should no longer exist and we can delete the state along with the skill class in order to follow OCP.(If we want to removed a skill, we can't leave any trace of the skill.)
- **deadAction : to record what a unit needs to do when it dies.**
    - Use `execute()` to execute the action the unit needs to do.
- **Skill implementation :**
    - Every skill has a method called `createSkill`, once the `testSkill` know that a unit has a certain skill from the inputs, it will use `createSkill()` to creat an skill object and add it into the `skillList` of the unit.
    - For those normal skill I simply use polymorphism and inheritance to use satisfy OCP. (Let every skill extend the skill class.)
    - **`SelfHealing` :** For the skill `SelfHealing`, I set the `targetList` to contain only one Unit that is the one which calls `SelfHealing`, and when you use `perform()`, the skill will add 150 to the HP.
    - **`Summon` :** For the skill `Summon`, we can directly add a slime into the troop that the unit who call `Summon` belongs to because I will sent `troop1` and `troop2` into every skill class during the initialization of the skill.
    - **`Petrochemical` :** For the skill `Petrochemical`, we directly assign a state `Petrochemical_st` to the targets. The state class is wrapped in `Petrochemical.java` so that once the skill nedd to be removed the state class `Petrochemical` can be removed together.
    - **`Curse` :** For the skill `Curse`, I wrap a class called `CurseAction` extending `deadAction` , when a Unit uses `Curse` on another unit, it will add a `CurseAction` into the `deadActions` of the cursed unit. Every time a Unit dies, it will execute all the deadAction in the `deadActions`. If the unit is cursed, the `CurseAction` will add the remaining MP of the cursed unit to the HP of the unit which cursed it. If we want the skill to be removed, we can just delete `Curse.java` and it satisfies OCP.(If we want to removed a skill, we can't leave any trace of the skill.)
    - **`Cheerup` :** For the skill `Cheerup`, I assign a state called `Cheerup_st` to the targets. All the states record a value called `HP_damageBonus`, and the `HP_damageBonus` of `Cheerup_st` will be nonzero while `HP_damageBonus` of the other states should be zero.
    - **`OnePunch` :** For the skill `OnePunch`, I create a class call `OnePunch_skill` extending `skill`. Inside the file `OnePunch_skill.java`, I wrap a class called `toGetOnePunch` which implements `Target`. I send the unit which is punched into the class `toGetOnePunch` and let its HP be damaged by using `takeOnePunchDamage(int damage)`. If we want the skill to be removed, we can just delete `OnePunch_skill.java` and it satisfies OCP.
- **How to add a new state or skill and still satisfy OCP?**
    - We just modify the initialization procedure within our program’s entry. Just add an object of the new skill as a `testSkill` and add it into the `testSkillList`.