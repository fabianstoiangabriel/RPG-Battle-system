#include <iostream>
#include <memory>
#include <vector>

class Character
{
    protected:
 std::string name;
 int hp;
 int atk;
    public:
    Character(std::string name , int hp , int atk) {this-> name = name; this-> hp = hp; this-> atk = atk;}

    void heal( int amount){hp+=amount;}

     void boostAttack(int amount){atk +=amount;}

     bool isAlive() {return hp > 0;}

     void takeDamage(int amount) {hp -= amount; std::cout<< name<< "took" <<amount<< "damage HP:"<<hp <<"\n";}

    std::string getName() {return name;}
    int getHP() {return hp;}
    int getATK(){return atk;}


};



class Items
{ std::string nameItem;
    int countItems;
     public:
     Items(std::string nameItem , int countItems){this->nameItem = nameItem ; this-> countItems = countItems;}
    virtual void use(Character& target) =0;
void decrement(){countItems-- ;}
int getCountItems(){return countItems;}

    std::string getNameItem(){return nameItem;}
};

class HealthPotion: public Items
{public:
   HealthPotion():Items("Health potion" , 3){}

   virtual void use(Character& target)override
   {int healthPotion = 25;
   target.heal(healthPotion);
    std::cout<<target.getName()<< "Health increased by" << healthPotion<<std::endl;

   }

};


class AttackPotion: public Items
{ public:
   AttackPotion():Items("Attack potion" , 3){}

   virtual void use(Character& target)override
   {int atkPotion = 9;
   target.boostAttack(atkPotion);
    std::cout<<target.getName()<< "Attack increased by" << atkPotion<<std::endl;

   }


};



template <typename T>
class Inventory
{std::vector<T> items;
    public:
void addItem(T item) {items.push_back(item);}

void useItem(int index , Character& target) {
    if(items[index] -> getCountItems() >0){
    items[index] -> use(target);
    items[index] ->decrement();}
    else{
    std::cout<< "No more items!"<<std::endl;
    }
}

void showItem()
{
    for( int i = 0 ;  i<items.size() ; i++)
    {
        std::cout<< i+1<< items[i]->getNameItem()<< "x"  << items[i]->getCountItems() <<std::endl;
    }
}
 bool isEmpty() {return items.empty();}
};

class Hero:public Character
{ int mana;
    public:
   Inventory<Items*> inventory;

Hero( int mana):Character("Saphir" , 100 , 10) , mana(mana) {}

void reduceMana(int amount) {mana -= amount;}

int getMana() {return mana;}
};

class Enemy:public Character
{
    public:
    Enemy(std::string name , int hp , int atk):Character(name , hp ,  atk){}
};



class CharacterFactory
{
    public:
    static std::unique_ptr<Character> createGoblin()
    { return std::make_unique<Enemy> ("Mr.Goblin" , 20 , 5);

    }
    static std::unique_ptr<Character> createGnome()
    {
        return std::make_unique<Enemy> ("Dumb Gnome" , 35 , 10);
    }

    static std::unique_ptr<Character> createBoss()
    {
        return std::make_unique<Enemy> ("Samael" , 100 , 12);
    }


};


class AttackStrategy
 { public:
    virtual void  execute(Character& attacker ,Character& target) = 0;
};

class BaseAtk:public AttackStrategy {
public:
    virtual void execute(Character&attacker , Character& target) override
    {int damage = attacker.getATK();
     target.takeDamage(damage);
    }

};

class FireMagicAtk: public AttackStrategy
{ public:
   virtual void execute(Character& attacker , Character& target) override
   {   int fireDamage =  13;
        target.takeDamage(fireDamage);
   }
};

class ElectricMagicAtk: public AttackStrategy
{ public:
   virtual void execute(Character& attacker ,Character& target) override
   {   int electricDamage =  18;
     target.takeDamage(electricDamage);
   }
};




template <typename T>
void logEvent(T value)
{
    std::cout<<"LOG"<<value<<std::endl;
}



int main()
{
    Hero hero1(80);
    BaseAtk baseAtk;
    FireMagicAtk fireAtk;
    ElectricMagicAtk electricAtk;
    HealthPotion healingPotion;
    AttackPotion atkIncreasePotion;
    hero1.inventory.addItem(&healingPotion);
    hero1.inventory.addItem(&atkIncreasePotion);



    std::cout<<"Pick your enemy: \n1. Goblin\n2.Gnome\n3.Boss\n ";
    int choice;
    std::cin>>choice;

    std::unique_ptr<Character> enemy;
    if(choice == 1) enemy = CharacterFactory::createGoblin();
    else if(choice == 2) enemy = CharacterFactory::createGnome();
    else enemy = CharacterFactory::createBoss();

logEvent("Battle started");
logEvent(hero1.getHP());

while(hero1.isAlive() && enemy -> isAlive())
{ std::cout<< "Saphir HP:" << hero1.getHP()<<  "Saphir Mana:" << hero1.getMana() <<std::endl;
    std::cout<< enemy ->getName()<< "HP:" <<enemy -> getHP()<<std::endl;

    std::cout<< "1.Basic Attack\n2.Fire Attack\n3.Electric Attack\n4.Use Items\n";
    int action;
    std::cin>>action;

  switch (action)
  {
  case 1:
    baseAtk.execute(hero1 , *enemy);
     break;
     case 2:
     if(hero1.getMana() >= 10 ){
     fireAtk.execute(hero1,*enemy);
     hero1.reduceMana(10);
     }
     else{
        std::cout<<"Not enough mana!"<<std::endl;
        continue;
     }

     break;

     case 3:
    if(hero1.getMana() >= 25){
      electricAtk.execute(hero1 , *enemy);
      hero1.reduceMana(25);
       }
       else{std::cout<<"Not enough mana!"<<std::endl; continue;}

     break;

     case 4:
     if(hero1.inventory.isEmpty())
     {std::cout<< "Inventory is empty!"<<std::endl;  }
     else{
     hero1.inventory.showItem();
     std::cout<<"Choose item\n1. Healing potion\n2.Increase Attack potion"<<std::endl;
     int itemChoice;
     std::cin>>itemChoice;
      hero1.inventory.useItem(itemChoice - 1 , hero1); continue;}

      break;
  default:{std::cout<<"Error"<<std::endl;  continue;}

break;  }


  hero1.takeDamage(enemy ->getATK());

}

if(hero1.isAlive())
std::cout<<"YOU WIN\n";
else
std::cout<< "you lose\n";



}
