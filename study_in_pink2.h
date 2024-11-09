/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class Robot;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

class MapElement
{
protected:
    ElementType type;

public:
    friend class TestStudyInPink;
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
public:
    friend class TestStudyInPink;

    Path();
};

class Wall : public MapElement
{
public:
    // Constructor of class Wall
    friend class TestStudyInPink;
    Wall();
};

class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    friend class TestStudyInPink;

    FakeWall(int in_req_exp);

    // Method getReqExp
    int getReqExp() const;
};

class Position
{
private:
    int r, c;

public:
    friend class TestStudyInPink;
    static const Position npos;
    Position(int r = 0, int c = 0);

    Position(const string &str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
};

class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    friend class TestStudyInPink;
    MovingObject(int index, const Position pos, Map *map, const string &name = "");

    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    string getName() const;
};

class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    friend class TestStudyInPink;
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class Character : public MovingObject
{
private:
    string name;
    int hp, exp;

public:
    friend class TestStudyInPink;
    Character(int index, const Position &pos, Map *map, const std::string &name = "");
    int getHp() const;
    int getExp() const;
    virtual void setHp(int hp);
    virtual void setExp(int exp);
};

class Sherlock : public Character
{
    /* TODO */
private:
    // TODO
    int hp, exp;
    const string moving_rule;
    int move_index;

public:
    friend class TestStudyInPink;
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    // Position getCurrentPosition() const;
    Position getNextPosition() override; // getNextPosition
    void move() override;                // move
    string str() const override;         // str
    void setHp(int hp) override;
    void setExp(int exp) override;
    // ...
};

class Watson : public Character
{
    /* TODO */
private:
    // TODO
    int hp, exp;
    const string moving_rule;
    int move_index;

public:
    friend class TestStudyInPink;
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    // Position getCurrentPosition() const;
    Position getNextPosition() override; // getNextPosition
    void move() override;                // move
    string str() const override;         // str
    void setHp(int hp) override;
    void setExp(int exp) override; // ...
};

class Criminal : public Character
{
    /* TODO */
private:
    // TODO Position getNextPosition() override;// getNextPosition
    Sherlock *sherlock;
    Watson *watson;
    int stepCount;
    Position prev_pos;
    // ...

public:
    friend class TestStudyInPink;
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    // Position getCurrentPosition() const;
    Position getNextPosition() override; // getNextPosition
    void move() override;                // move
    string str() const override;         // str
    int getStepCount() const;
    Position getPrevPosition() const;
    // ...
};

class ArrayMovingObject
{
private:
    // TODO
    MovingObject **arr_mv_objs;
    int count = 0, capacity;

public:
    friend class TestStudyInPink;
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
    int getIndex() const;
};

class Configuration
{
    friend class StudyPinkProgram;

private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls, num_fake_walls, num_steps;
    Position *arr_walls, *arr_fake_walls;
    string sherlock_moving_rule, watson_moving_rule;
    Position sherlock_init_pos, watson_init_pos, criminal_init_pos;
    int sherlock_init_hp, sherlock_init_exp, watson_init_hp, watson_init_exp;
    // TODO

public:
    friend class TestStudyInPink;
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

class BaseItem // BaseItem
{
private:
    ItemType itemtype;

public:
    friend class TestStudyInPink;
    BaseItem(ItemType itemtype);
    virtual ~BaseItem();
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    virtual ItemType getType() const = 0;

    virtual string str() const = 0;
};

class Robot : public MovingObject // Robot
{
private:
    RobotType robot_type;
    BaseItem *item;
    bool FirstRobot = true;

public:
    friend class TestStudyInPink;
    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type);
    virtual ~Robot();

    RobotType getRobotType() const;

    bool addRobot(Criminal *criminal, ArrayMovingObject *arr_mv_objs, Sherlock *sherlock, Watson *watson);

    BaseItem *getKeepItem() const;
    void setKeepItem(Robot *robot);

    int findS(int r, int c);
};

class RobotC : public Robot
{
private:
    Criminal *criminal;

public:
    friend class TestStudyInPink;

    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
    ~RobotC() override;
    // Position getCurrentPosition();
    Position getNextPosition() override;
    void move() override;
    int getDistance(const Sherlock *sherlock) const;
    int getDistance(const Watson *watson) const;
    string str() const override;
};

class RobotS : public Robot
{
private:
    Criminal *criminal;
    Sherlock *sherlock;

public:
    friend class TestStudyInPink;
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock);
    ~RobotS() override;
    // Position getCurrentPosition();
    Position getNextPosition() override;
    void move() override;
    int getDistance() const;
    string str() const override;
};

class RobotW : public Robot
{
private:
    Criminal *criminal;
    Watson *watson;

public:
    friend class TestStudyInPink;
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
    ~RobotW() override;
    // Position getCurrentPosition();
    Position getNextPosition() override;
    void move() override;
    int getDistance() const;
    string str() const override;
};

class RobotSW : public Robot
{
private:
    Criminal *criminal;
    Sherlock *sherlock;
    Watson *watson;

public:
    friend class TestStudyInPink;
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    ~RobotSW() override;
    // Position getCurrentPosition();
    Position getNextPosition() override;
    void move();
    int getDistance() const;
    string str() const override;
};

class MagicBook : public BaseItem
{
    public:
    MagicBook();

    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class EnergyDrink : public BaseItem
{
    public: 
    EnergyDrink();

    friend class TestStudyInPink;
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class FirstAid : public BaseItem
{
    public:
    FirstAid();

    friend class TestStudyInPink;
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class ExcemptionCard : public BaseItem
{
    public:
    ExcemptionCard();

    friend class TestStudyInPink;
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class PassingCard : public BaseItem
{
private:
    string challenge;

public:
    PassingCard(string challenge);

    friend class TestStudyInPink;
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    ItemType getType() const override;
    string str() const override;
};

class BaseBag
{
protected:
    struct Node
    {
        BaseItem *item;
        Node *next;
        Node(BaseItem *item, Node *next = nullptr) : item(item), next(nullptr) {}
    };

    Character *obj;
    Node *head;
    int count;
    int maxNum;

public:
    friend class TestStudyInPink;

    BaseBag(Character *obj, int maxNum);
    virtual ~BaseBag();

    virtual bool insert(BaseItem *item);      // return true if insert successfully
    virtual BaseItem *get();                  // return the item as described above , if not found , return NULL
    virtual BaseItem *get(ItemType itemType); // return the item as described above , if not found , return NULL
    virtual string str() const;
    Node *find(ItemType item) const;
    void remove(Node *prev, Node *current);

}; // BaseBag,...

class SherlockBag : public BaseBag
{
private:
public:
    SherlockBag(Sherlock *sherlock);
    bool insert(BaseItem *item) override;
    BaseItem *get() override;
    BaseItem *get(ItemType itemType) override;
    // string str() const;
};

class WatsonBag : public BaseBag
{
private:
public:
    friend class TestStudyInPink;

    WatsonBag(Watson *watson);
    bool insert(BaseItem *item);
    BaseItem *get() override;
    BaseItem *get(ItemType itemType) override;
    // string str() const;
    
};

class StudyPinkProgram
{
private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

    Robot *robot;
    BaseBag *basebag1, *basebag2;


public:
    friend class TestStudyInPink;

    StudyPinkProgram(const string &config_file_path);

    bool isStop() const;

    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose);

    ~StudyPinkProgram();

    void handleEncounter(Character *character1, Character *character2, Criminal *criminal, Robot *robot, BaseBag *basebag1, BaseBag *basebag2);
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /*_H_STUDY_IN_PINK_2_H_*/
