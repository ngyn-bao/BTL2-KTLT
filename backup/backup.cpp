#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

/*MAP ELEMENT*/
MapElement::MapElement(ElementType in_type) : type(in_type) {}
MapElement::~MapElement() {}
ElementType MapElement::getType() const
{
    return type;
}

/*PATH*/
Path::Path() : MapElement(ElementType::PATH) {}

/*WALL*/
Wall::Wall() : MapElement(ElementType::WALL) {}

/*FAKE WALL*/

FakeWall::FakeWall(int in_req_exp) : MapElement(ElementType::FAKE_WALL), req_exp(in_req_exp) {}

int FakeWall::getReqExp() const
{
    return req_exp;
}

/*MAP*/
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls) : num_rows(num_rows), num_cols(num_cols)
{
    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++)
        {
            delete map[i][j];
            map[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_walls; i++)
    {
        delete map[array_walls[i].getRow()][array_walls[i].getCol()];
        map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; i++)
    {
        delete map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()];
        int in_req_exp = (array_fake_walls[i].getRow() * 257 + array_fake_walls[i].getRow() * 139 + 89) % 900 + 1;
        map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] = new FakeWall(in_req_exp);
    }
}

Map::~Map() // Destructor
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    if (pos.getRow() >= 0 && pos.getRow() < num_rows && pos.getCol() >= 0 && pos.getCol() < num_cols)
    {
        MapElement *element = map[pos.getRow()][pos.getCol()];

        if (element->getType() == WALL)
            return false;
        else if (element->getType() == FAKE_WALL)
        {
            if (mv_obj->getName() == "Sherlock")
                return true;
            else if (mv_obj->getName() == "Criminal")
                return true;
            else if (mv_obj->getName() == "Robot")
                return true;
            else if (mv_obj->getName() == "Watson")
            {
                int exp_req = dynamic_cast<FakeWall *>(element)->getReqExp();
                Watson *watson = dynamic_cast<Watson *>(mv_obj);
                if (watson->getExp() > exp_req)
                {
                    return true;
                }
                else
                    return false;
            }
            return false;
        }
        return true;
    }
    return false;
}

/*POSITION*/
Position::Position(int r, int c) : r(r), c(c) {}

Position::Position(const string &str_pos)
{
    char ch;
    istringstream pos_stream(str_pos);
    pos_stream >> ch >> r >> ch >> c >> ch;
    this->r = r;
    this->c = c;
}

int Position::getRow() const
{
    return r;
}

int Position::getCol() const
{
    return c;
}

void Position::setRow(int r) // Cap nhat bien r
{
    this->r = r;
}

void Position::setCol(int c) // Cap nhat bien c
{
    this->c = c;
}

string Position::str() const // Bieu dien thong tin vi tri
{
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const
{ // Neu tham so truyen vao trung voi vi tri cua doi tuong thi tra ve True, nguoc lai la False
    if (in_r == r && in_c == c)
        return true;
    return false;
}

const Position Position::npos = Position(-1, -1);

/*MOVING OBJECT*/
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name) : index(index), pos(pos), map(map), name(name) {}
MovingObject::~MovingObject() {}
string MovingObject::getName() const
{
    return name;
}
Position MovingObject::getCurrentPosition() const
{
    return pos;
}
/*SHERLOCK*/
Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), hp(init_hp), exp(init_exp), moving_rule(moving_rule), move_index(0)
{
    if (hp < 0)
        hp = 0;
    if (hp > 500)
        hp = 500;
    if (exp < 0)
        exp = 0;
    if (exp > 900)
        exp = 900;
}

Position Sherlock::getNextPosition()
{
    if (moving_rule.empty())
    {
        return Position::npos;
    }

    char move = moving_rule[move_index];
    move_index = (move_index + 1) % moving_rule.size();

    Position next_pos = pos;
    switch (move)
    {
    case 'U':
        next_pos.setRow(pos.getRow() - 1);
        break;
    case 'R':
        next_pos.setCol(pos.getCol() + 1);
        break;
    case 'D':
        next_pos.setRow(pos.getRow() + 1);
        break;
    case 'L':
        next_pos.setCol(pos.getCol() - 1);
        break;
    default:
        return next_pos = Position::npos;
    }

    if (map->isValid(next_pos, this))
    {
        return next_pos;
    }
    else
    {
        return Position::npos;
    }
}

void Sherlock::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        pos = next_pos;
    }
}

string Sherlock::str() const
{
    return "Sherlock[index=" + std::to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

void Sherlock::setHp(int hp)
{
    this->hp = hp;
}

void Sherlock::setExp(int exp)
{
    this->exp = exp;
}

/*WATSON*/
Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), hp(init_hp), exp(init_exp), moving_rule(moving_rule), move_index(0)
{
    if (hp < 0)
        hp = 0;
    if (hp > 500)
        hp = 500;
    if (exp < 0)
        exp = 0;
    if (exp > 900)
        exp = 900;
}

Position Watson::getNextPosition()
{
    if (moving_rule.empty())
    {
        return Position::npos;
    }

    char move = moving_rule[move_index];
    move_index = (move_index + 1) % moving_rule.size();

    Position next_pos = pos;
    switch (move)
    {
    case 'U':
        next_pos.setRow(pos.getRow() - 1);
        break;
    case 'R':
        next_pos.setCol(pos.getCol() + 1);
        break;
    case 'D':
        next_pos.setRow(pos.getRow() + 1);
        break;
    case 'L':
        next_pos.setCol(pos.getCol() - 1);
        break;
    default:
        return next_pos = Position::npos;
    }

    if (map->isValid(next_pos, this))
    {
        return next_pos;
    }
    else
    {
        return Position::npos;
    }
}

void Watson::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        pos = next_pos;
    }
}

string Watson::str() const
{
    return "Watson[index=" + std::to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

void Watson::setHp(int hp)
{
    this->hp = hp;
}

void Watson::setExp(int exp)
{
    this->exp = exp;
}

/*CRIMINAL*/
Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson), stepCount(0), prev_pos(Position::npos) {}

Position Criminal::getNextPosition()
{
    Position sherlock_pos = sherlock->getCurrentPosition(), watson_pos = watson->getCurrentPosition(), max_distance_pos = pos;
    int max_distance = -1;

    Position next_pos = Position(pos.getRow() - 1, pos.getCol());

    int distance = abs(next_pos.getRow() - sherlock_pos.getRow()) + abs(next_pos.getCol() - sherlock_pos.getCol()) + abs(next_pos.getRow() - watson_pos.getRow()) + abs(next_pos.getCol() - watson_pos.getCol());

    if (distance > max_distance)
    {
        max_distance = distance;
        max_distance_pos = next_pos;
    }

    next_pos = Position(pos.getRow(), pos.getCol() - 1);

    distance = abs(next_pos.getRow() - sherlock_pos.getRow()) + abs(next_pos.getCol() - sherlock_pos.getCol()) + abs(next_pos.getRow() - watson_pos.getRow()) + abs(next_pos.getCol() - watson_pos.getCol());

    if (distance > max_distance)
    {
        max_distance = distance;
        max_distance_pos = next_pos;
    }

    next_pos = Position(pos.getRow() + 1, pos.getCol());
    distance = abs(next_pos.getRow() - sherlock_pos.getRow()) + abs(next_pos.getCol() - sherlock_pos.getCol()) + abs(next_pos.getRow() - watson_pos.getRow()) + abs(next_pos.getCol() - watson_pos.getCol());

    if (distance > max_distance)
    {
        max_distance = distance;
        max_distance_pos = next_pos;
    }

    next_pos = Position(pos.getRow(), pos.getCol() + 1);
    distance = abs(next_pos.getRow() - sherlock_pos.getRow()) + abs(next_pos.getCol() - sherlock_pos.getCol()) + abs(next_pos.getRow() - watson_pos.getRow()) + abs(next_pos.getCol() - watson_pos.getCol());

    if (distance > max_distance)
    {
        max_distance = distance;
        max_distance_pos = next_pos;
    }
    return max_distance_pos;
}

void Criminal::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        prev_pos = pos;
        pos = next_pos;
        stepCount++;
    }
}

string Criminal::str() const
{
    return "Criminal[index=" + std::to_string(index) + ";pos=" + pos.str() + "]";
}

int Criminal::getStepCount() const
{
    return stepCount;
}

Position Criminal::getPrevPosition() const
{
    return prev_pos;
}

/*ARRAY MOVING OBJECT*/
ArrayMovingObject::ArrayMovingObject(int capacity) : count(0), capacity(capacity)
{
    arr_mv_objs = new MovingObject *[capacity];
}

ArrayMovingObject::~ArrayMovingObject()
{
    for (int i = 3; i < count; i++)
    {
        delete arr_mv_objs[i];
    }
    delete[] arr_mv_objs;
}

bool ArrayMovingObject::isFull() const
{
    return count == capacity;
}

bool ArrayMovingObject::add(MovingObject *mv_objs)
{
    if (!isFull())
    {
        arr_mv_objs[count] = mv_objs;
        count++;
        return true;
    }
    return false;
}

int ArrayMovingObject::size() const
{
    return count;
}

MovingObject *ArrayMovingObject::get(int index) const
{
    if (index >= 0 && index < count)
        return arr_mv_objs[index];
    return nullptr;
}

string ArrayMovingObject::str() const
{
    stringstream ss;
    ss << "ArrayMovingObject[count=" << to_string(count) << ";capacity=" << to_string(capacity) << ";";
    for (int i = 0; i < size(); i++)
    {
        if (arr_mv_objs[i] != nullptr)
        {
            ss << arr_mv_objs[i]->str();
            if (i < size() - 1)
                ss << ";";
        }
    }
    ss << "]";
    return ss.str();
}

/*CONFIGURATION*/
Configuration::Configuration(const string &filepath) : map_num_rows(0), map_num_cols(0), max_num_moving_objects(0), num_walls(0), arr_walls(nullptr), num_fake_walls(0), arr_fake_walls(nullptr), sherlock_init_hp(0), sherlock_init_exp(0), watson_init_hp(0), watson_init_exp(0), num_steps(0)
{
    ifstream file(filepath);
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string key, value;
        getline(iss, key, '=');
        getline(iss, value);

        if (key == "MAP_NUM_ROWS")
        {
            map_num_rows = stoi(value);
        }

        else if (key == "MAP_NUM_COLS")
        {
            map_num_cols = stoi(value);
        }

        else if (key == "MAX_NUM_MOVING_OBJECTS")
        {
            max_num_moving_objects = stoi(value);
        }

        else if (key == "ARRAY_WALLS")
        {
            istringstream arr_stream(value.substr(1, value.length() - 2));
            string posStr;
            while (getline(arr_stream, posStr, ';'))
            {
                ++num_walls;
            }
            arr_walls = new Position[num_walls];
            arr_stream.clear();
            arr_stream.seekg(0, ios::beg);
            int i = 0;
            while (getline(arr_stream, posStr, ';') && i < num_walls)
            {
                arr_walls[i++] = Position(posStr);
            }
        }

        else if (key == "ARRAY_FAKE_WALLS")
        {
            istringstream arr_stream(value.substr(1, value.length() - 2));
            string posStr;
            while (getline(arr_stream, posStr, ';'))
            {
                ++num_fake_walls;
            }
            arr_fake_walls = new Position[num_fake_walls];
            arr_stream.clear();
            arr_stream.seekg(0, ios::beg);
            int i = 0;
            while (getline(arr_stream, posStr, ';') && i < num_fake_walls)
            {
                arr_fake_walls[i++] = Position(posStr);
            }
        }

        else if (key == "SHERLOCK_INIT_POS")
        {
            sherlock_init_pos = Position(value);
        }

        else if (key == "SHERLOCK_MOVING_RULE")
        {
            sherlock_moving_rule = value;
        }

        else if (key == "SHERLOCK_INIT_HP")
        {
            sherlock_init_hp = stoi(value);
        }

        else if (key == "SHERLOCK_INIT_EXP")
        {
            sherlock_init_exp = stoi(value);
        }

        else if (key == "WATSON_INIT_POS")
        {
            watson_init_pos = Position(value);
        }

        else if (key == "WATSON_MOVING_RULE")
        {
            watson_moving_rule = value;
        }

        else if (key == "WATSON_INIT_HP")
        {
            watson_init_hp = stoi(value);
        }

        else if (key == "WATSON_INIT_EXP")
        {
            watson_init_exp = stoi(value);
        }

        else if (key == "CRIMINAL_INIT_POS")
        {
            criminal_init_pos = Position(value);
        }
        else if (key == "NUM_STEPS")
        {
            num_steps = stoi(value);
        }
    }
}

Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
}

string Configuration::str() const
{
    stringstream ss;
    ss << "Configuration[" << endl;
    ss << "MAP_NUM_ROWS=" << map_num_rows << endl;
    ss << "MAP_NUM_COLS=" << map_num_cols << endl;
    ss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << endl;
    ss << "NUM_WALLS=" << num_walls << endl;
    ss << "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; ++i)
    {
        if (i > 0)
            ss << ";";
        ss << arr_walls[i].str();
    }
    ss << "]" << endl;
    ss << "NUM_FAKE_WALLS=" << num_fake_walls << endl;
    ss << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; ++i)
    {
        if (i > 0)
            ss << ";";
        ss << arr_fake_walls[i].str();
    }
    ss << "]" << endl;
    ss << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << endl;
    ss << "SHERLOCK_INIT_POS=(" << sherlock_init_pos.getRow() << "," << sherlock_init_pos.getCol() << ")" << endl;
    ss << "SHERLOCK_INIT_HP=" << sherlock_init_hp << endl;
    ss << "SHERLOCK_INIT_EXP=" << sherlock_init_exp << endl;
    ss << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
    ss << "WATSON_INIT_POS=(" << watson_init_pos.getRow() << "," << watson_init_pos.getCol() << ")" << endl;
    ss << "WATSON_INIT_HP=" << watson_init_hp << endl;
    ss << "WATSON_INIT_EXP=" << watson_init_exp << endl;
    ss << "CRIMINAL_INIT_POS=(" << criminal_init_pos.getRow() << "," << criminal_init_pos.getCol() << ")" << endl;
    ss << "NUM_STEPS=" << num_steps << endl;
    ss << "]";
    return ss.str();
}

/*ROBOT*/
Robot::Robot(int index, const Position &init_pos, Map *map, RobotType robot_type) : MovingObject(index, init_pos, map, "Robot"), robot_type(robot_type) {}

Robot::~Robot() {}

RobotType Robot::getRobotType() const
{
    return robot_type;
}

bool Robot::addRobot(Criminal *criminal, ArrayMovingObject *arr_mv_objs, Sherlock *sherlock, Watson *watson)
{
    if (criminal->getStepCount() == 3)
    {
        Robot *robot = nullptr;

        if (FirstRobot)
        {
            // add
            robot = new RobotC(index, criminal->getPrevPosition(), map, criminal);
            if (!arr_mv_objs->isFull())
            {
                arr_mv_objs->add(robot);
                robot->setKeepItem(robot);
                FirstRobot = false;
                return true;
            }
        }
        else
        {
            int distance_to_sherlock = abs(robot->getCurrentPosition().getRow() - sherlock->getCurrentPosition().getRow()) + abs(robot->getCurrentPosition().getCol() - sherlock->getCurrentPosition().getCol());
            int distance_to_watson = abs(robot->getCurrentPosition().getRow() - watson->getCurrentPosition().getRow()) + abs(robot->getCurrentPosition().getCol() - watson->getCurrentPosition().getCol());

            if (distance_to_sherlock < distance_to_watson)
            {
                robot = new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
                if (!arr_mv_objs->isFull())
                {
                    arr_mv_objs->add(robot);
                    robot->setKeepItem(robot);
                    return true;
                }
            }

            else if (distance_to_sherlock > distance_to_watson)
            {
                robot = new RobotW(index, criminal->getPrevPosition(), map, criminal, watson);
                if (!arr_mv_objs->isFull())
                {
                    arr_mv_objs->add(robot);
                    robot->setKeepItem(robot);
                    return true;
                }
            }

            else if (distance_to_sherlock == distance_to_watson)
            {
                robot = new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
                if (!arr_mv_objs->isFull())
                {
                    arr_mv_objs->add(robot);
                    robot->setKeepItem(robot);
                    return true;
                }
            }
        }
    }
    return false;
}

BaseItem *Robot::getKeepItem() 
{
    return item;
}

void Robot::setKeepItem(Robot *robot)
{
    delete item;

    if (findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) >= 0 && findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) <= 1)
        item = new MagicBook();
    else if (findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) >= 2 && findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) <= 3)
        item = new EnergyDrink();
    else if (findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) >= 4 && findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) <= 5)
        item = new FirstAid();
    else if (findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) >= 6 && findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) <= 7)
        item = new ExcemptionCard();
    else if (findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) >= 8 && findS(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) <= 9)
        item = new PassingCard();
    else item = nullptr;
}
// ROBOTC
RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, C), criminal(criminal) {}
RobotC::~RobotC() {}

Position RobotC::getNextPosition()
{
    return criminal->getPrevPosition();
}

void RobotC::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        pos = next_pos;
    }
}

int RobotC::getDistance(const Sherlock *sherlock) const
{
    int sherlock_distance = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());

    return sherlock_distance;
}

int RobotC::getDistance(const Watson *watson) const
{
    int watson_distance = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());

    return watson_distance;
}

string RobotC::str() const
{
    return "Robot[pos=" + pos.str() + ";type=C;dist=" + string() + "]";
}

// ROBOTS
RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock) : Robot(index, init_pos, map, S), criminal(criminal), sherlock(sherlock) {}
RobotS::~RobotS() {}

Position RobotS::getNextPosition()
{
    Position next_pos(pos.getRow() - 1, pos.getCol());
    int min_distance = abs(next_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(next_pos.getCol() - sherlock->getCurrentPosition().getCol());

    Position canbenext_pos(pos.getRow(), pos.getCol() + 1);
    int distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow() + 1, pos.getCol());
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow(), pos.getCol() - 1);
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    return next_pos;
}

void RobotS::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        pos = next_pos;
    }
}

int RobotS::getDistance() const
{
    int sherlock_distance = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
    return sherlock_distance;
}

string RobotS::str() const
{
    int sherlock_distance = getDistance();
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(sherlock_distance) + "]";
}

// ROBOTW
RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map, S), criminal(criminal), watson(watson) {}
RobotW::~RobotW() {}

Position RobotW::getNextPosition()
{
    Position next_pos(pos.getRow() - 1, pos.getCol());
    int min_distance = abs(next_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(next_pos.getCol() - watson->getCurrentPosition().getCol());

    Position canbenext_pos(pos.getRow(), pos.getCol() + 1);
    int distance = abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow() + 1, pos.getCol());
    distance = abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow(), pos.getCol() - 1);
    distance = abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    return next_pos;
}

void RobotW::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        pos = next_pos;
    }
}

int RobotW::getDistance() const
{
    int watson_distance = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
    return watson_distance;
}

string RobotW::str() const
{
    int watson_distance = getDistance();
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(watson_distance) + "]";
}

// ROBOTSW
RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson) : Robot(index, init_pos, map, S), criminal(criminal), sherlock(sherlock), watson(watson) {}
RobotSW::~RobotSW() {}

Position RobotSW::getNextPosition()
{
    Position next_pos(pos.getRow() - 2, pos.getCol());
    int min_distance = abs(next_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(next_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());

    Position canbenext_pos(pos.getRow() - 1, pos.getCol() + 1);
    int distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow(), pos.getCol() + 2);
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow() + 1, pos.getCol() + 1);
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow() + 2, pos.getCol());
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow() + 1, pos.getCol() - 1);
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow(), pos.getCol() - 2);
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    canbenext_pos = Position(pos.getRow() - 1, pos.getCol() - 1);
    distance = abs(canbenext_pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(canbenext_pos.getRow() - watson->getCurrentPosition().getRow()) + abs(canbenext_pos.getCol() - watson->getCurrentPosition().getCol());
    if (distance < min_distance)
    {
        min_distance = distance;
        next_pos = canbenext_pos;
    }

    return next_pos;
}

void RobotSW::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        pos = next_pos;
    }
}

int RobotSW::getDistance() const
{
    int sherlock_watson_distance = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
    return sherlock_watson_distance;
}

string RobotSW::str() const
{
    int sherlock_watson_distance = getDistance();
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(sherlock_watson_distance) + "]";
}

int Robot::findS(int r, int c)
{
    int p = r * c;
    int s = 0;
    if (p < 10)
        return p;
    else
    {

        while (p >= 10)
        {
            s += p % 10;
            p = p / 10;
        }
    }
    return s;
}
/*BASEITEM*/
// BASEITEM
BaseItem::BaseItem(ItemType itemtype) : itemtype(itemtype) {}
BaseItem::~BaseItem() {}
// CHARACTER
Character::Character(int index, const Position &pos, Map *map, const std::string &name) : MovingObject(index, pos, map, name) {}

int Character::getHp() const
{
    return hp;
}

int Character::getExp() const
{
    return exp;
}

void Character::setHp(int hp)
{
    this->hp = hp;
}

void Character::setExp(int exp)
{
    this->exp = exp;
}

// MAGICBOOK
MagicBook::MagicBook() : BaseItem(MAGIC_BOOK) {}


bool MagicBook::canUse(Character *obj, Robot *robot)
{
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    if (sherlock != nullptr)
    {
        if (sherlock->getExp() >= 0 && sherlock->getExp() <= 350)
            return true;
    }
    Watson *watson = dynamic_cast<Watson *>(obj);
    if (watson != nullptr)
    {
        if (watson->getExp() >= 0 && watson->getExp() <= 350)
        {
            return true;
        }
    }
    return false;
}

ItemType MagicBook::getType() const
{
    return MAGIC_BOOK;
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        obj->setExp(ceil((float)obj->getExp() + (float)25 / 100 * obj->getExp()));
    }
}

string MagicBook::str() const
{
    return "MagicBook";
}

// ENERGYDRINK
EnergyDrink::EnergyDrink() : BaseItem(ENERGY_DRINK) {}


bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    if (sherlock != nullptr)
    {
        if (sherlock->getHp() >= 0 && sherlock->getHp() <= 100)
            return true;
    }
    Watson *watson = dynamic_cast<Watson *>(obj);
    if (watson != nullptr)
    {
        if (watson->getHp() >= 0 && watson->getHp() <= 100)
        {
            return true;
        }
    }
    return false;
}

void EnergyDrink::use(Character *obj, Robot *robot)
{

    if (canUse(obj, robot))
    {
        obj->setHp(ceil((float)obj->getHp() + (float)20 / 100 * obj->getHp()));
    }
}

ItemType EnergyDrink::getType() const
{
    return ENERGY_DRINK;
}

string EnergyDrink::str() const
{
    return "EnergyDrink";
}

// FIRSTAID
FirstAid::FirstAid() : BaseItem(FIRST_AID) {}


bool FirstAid::canUse(Character *obj, Robot *robot)
{
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    if (sherlock != nullptr)
    {
        if ((sherlock->getHp() >= 0 && sherlock->getHp() <= 100) || (sherlock->getExp() >= 0 && sherlock->getExp() <= 350))
            return true;
    }
    Watson *watson = dynamic_cast<Watson *>(obj);
    if (watson != nullptr)
    {
        if ((watson->getHp() >= 0 && watson->getHp() <= 100) || (watson->getExp() >= 0 && watson->getExp() <= 350))
            return true;
    }
    return false;
}

void FirstAid::use(Character *obj, Robot *robot)
{

    if (canUse(obj, robot))
    {
        obj->setHp(ceil((float)obj->getHp() + (float)50 / 100 * obj->getHp()));
    }
}

ItemType FirstAid::getType() const
{
    return FIRST_AID;
}

string FirstAid::str() const
{
    return "FirstAid";
}

// EXCEMPTIONCARD
ExcemptionCard::ExcemptionCard() : BaseItem(EXCEMPTION_CARD) {}


bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    if (sherlock != nullptr && robot != nullptr)
    {
        if (sherlock->getHp() % 2 != 0)
            return true;
    }
    return false;
}

void ExcemptionCard::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        return;
    }
}

ItemType ExcemptionCard::getType() const
{
    return EXCEMPTION_CARD;
}

string ExcemptionCard::str() const
{
    return "ExcemptionCard";
}

// PASSINGCARD
PassingCard::PassingCard(string challenge) : BaseItem(PASSING_CARD), challenge(challenge) {}


bool PassingCard::canUse(Character *obj, Robot *robot)
{
    Watson *watson = dynamic_cast<Watson *>(obj);
    if (watson != nullptr && robot != nullptr)
    {
        if (watson->getHp() % 2 == 0)
            return true;
    }
    return false;
}

void PassingCard::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        int t = ((robot->getCurrentPosition().getRow() * 11 + robot->getCurrentPosition().getCol()) % 4);
        RobotType type;
        if (t == 0)
        {
            challenge = "RobotS";
            type = S;
        }
        else if (t == 1)
        {
            challenge = "RobotC";
            type = C;
        }
        else if (t == 2)
        {
            challenge = "RobotSW";
            type = SW;
        }
        else if (t == 3)
            challenge = "all";

        if (challenge != "all" && type != robot->getRobotType())
            obj->setExp(obj->getExp() - 50);
        else
            return;
    }
}

ItemType PassingCard::getType() const
{
    return PASSING_CARD;
}

string PassingCard::str() const
{
    return "PassingCard";
}

/*BASEBAG*/
// BASEBAG
BaseBag::BaseBag(Character *obj, int maxNum) : obj(obj), count(0), head(nullptr), maxNum(maxNum) {}

BaseBag::~BaseBag()
{
    Node *current = head;
    while (current)
    {
        Node *next = current->next;
        delete current->item;
        delete current;
        current = next;
    }
}

bool BaseBag::insert(BaseItem *item)
{
    if (count <= maxNum)
    {
        head = new Node(item, head);
        count++;
        return true;
    }
    return false;
}

BaseItem *BaseBag::get()
{
    Node *prev = nullptr;
    Node *current = head;
    while (current)
    {
        ItemType type = current->item->getType();
        if (type == EXCEMPTION_CARD || type == PASSING_CARD || type == MAGIC_BOOK || type == FIRST_AID || type == ENERGY_DRINK)
        {
            current->item->canUse(obj, nullptr);
            BaseItem *item = current->item;
            remove(prev, current);
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

BaseItem *BaseBag::get(ItemType itemType)
{
    Node *prev = find(itemType);
    if (prev)
    {
        Node *current = prev->next ? prev->next : head;
        if (current->item->canUse(obj, nullptr))
        {
            BaseItem *item = current->item;
            remove(prev, current);
            return item;
        }
    }
    return nullptr;
}

string BaseBag::str() const
{
    ostringstream ss;
    ss << "Bag[count=" << to_string(count) << ";";
    Node *current = head;
    while (current)
    {
        if (current != head)
        {
            ss << ",";
        }
        ss << current->item->str();
        current = current->next;
    }
    ss << "]";
    return ss.str();
}

BaseBag::Node *BaseBag::find(ItemType itemType) const
{
    Node *prev = nullptr;
    Node *current = head;
    while (current)
    {
        if (current->item->getType() == itemType)
        {
            return prev;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

void BaseBag::remove(Node *prev, Node *current)
{
    if (prev)
    {
        prev->next = current->next;
    }

    else
    {
        head = current->next;
    }
    delete current->item;
    delete current;
    --count;
}
// SHERLOCKBAG
SherlockBag::SherlockBag(Sherlock *sherlock) : BaseBag(sherlock, 13) {}

bool SherlockBag::insert(BaseItem *item)
{
    if (count <= 13)
    {
        head = new Node(item, head);
        count++;
        return true;
    }
    return false;
}

BaseItem *SherlockBag::get()
{
    Node *prev = nullptr;
    Node *current = head;
    while (current)
    {
        ItemType type = current->item->getType();
        if (type == EXCEMPTION_CARD || type == PASSING_CARD || type == MAGIC_BOOK || type == FIRST_AID || type == ENERGY_DRINK)
        {
            current->item->canUse(obj, nullptr);
            BaseItem *item = current->item;
            remove(prev, current);
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

BaseItem *SherlockBag::get(ItemType itemType)
{
    Node *prev = find(itemType);
    if (prev)
    {
        Node *current = prev->next ? prev->next : head;
        if (current->item->canUse(obj, nullptr))
        {
            BaseItem *item = current->item;
            remove(prev, current);
            return item;
        }
    }
    return nullptr;
}

// WATSONBAG
WatsonBag::WatsonBag(Watson *watson) : BaseBag(watson, 15) {}

bool WatsonBag::insert(BaseItem *item)
{
    if (count <= 15)
    {
        head = new Node(item, head);
        count++;
        return true;
    }
    return false;
}

BaseItem *WatsonBag::get()
{
    Node *prev = nullptr;
    Node *current = head;
    while (current)
    {
        ItemType type = current->item->getType();
        if (type == EXCEMPTION_CARD || type == PASSING_CARD || type == MAGIC_BOOK || type == FIRST_AID || type == ENERGY_DRINK)
        {
            if (current->item->canUse(obj, nullptr))
            {
                BaseItem *item = current->item;
                remove(prev, current);
                return item;
            }
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

BaseItem *WatsonBag::get(ItemType itemType)
{
    Node *prev = find(itemType);
    if (prev)
    {
        Node *current = prev->next ? prev->next : head;
        if (current->item->canUse(obj, nullptr))
        {
            BaseItem *item = current->item;
            remove(prev, current);
            return item;
        }
    }
    return nullptr;
}

/*STUDYPINKPROGRAM*/
StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
    robot = nullptr;
    basebag1 = new BaseBag(sherlock, 13);
    basebag2 = new BaseBag(watson, 15);
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete config;
    delete sherlock;
    delete watson;
    delete criminal;
    delete map;
    delete arr_mv_objs;
}

bool StudyPinkProgram::isStop() const
{
    if (sherlock->getHp() == 1 || watson->getHp() == 1 || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
        return true;
    return false;
}

void StudyPinkProgram::run(bool verbose)
{
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            if (arr_mv_objs->get(i) == nullptr)
                continue;
            else
            {
                arr_mv_objs->get(i)->move();

                handleEncounter(sherlock, watson, criminal, robot, basebag1, basebag2);
                if (isStop())
                {
                    printStep(istep);
                    break;
                }
                Robot *robot = nullptr;
                robot->addRobot(criminal, arr_mv_objs, sherlock, watson);
                if (verbose)
                {
                    printStep(istep);
                }
            }
        }
    }
    printResult();
}

void StudyPinkProgram::handleEncounter(Character *character1, Character *character2, Criminal *criminal, Robot *robot, BaseBag *basebag1, BaseBag *basebag2)
{
    Sherlock *sherlock = dynamic_cast<Sherlock *>(character1);
    Watson *watson = dynamic_cast<Watson *>(character2);
    SherlockBag *sherlockbag = dynamic_cast<SherlockBag *>(basebag1);
    WatsonBag *watsonbag = dynamic_cast<WatsonBag *>(basebag2);
    if (sherlock->getCurrentPosition().isEqual(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()) || watson->getCurrentPosition().isEqual(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()))
    {
        if (sherlock)
        {

            if (RobotS *robotS = dynamic_cast<RobotS *>(robot))
            {

                if (sherlock->getExp() > 400)
                {
                    sherlockbag->insert(robotS->getKeepItem());
                }
                else
                {
                    if (sherlockbag->find(EXCEMPTION_CARD))
                    {
                        sherlockbag->get(EXCEMPTION_CARD)->use(sherlock, robotS);
                        sherlockbag->insert(robotS->getKeepItem());
                    }
                    else
                        sherlock->setExp(ceil(sherlock->getExp() - sherlock->getExp() * (float)10 / 100));
                }
            }

            else if (RobotW *robotW = dynamic_cast<RobotW *>(robot))
            {
                return;
            }

            else if (RobotSW *robotSW = dynamic_cast<RobotSW *>(robot))
            {

                if (sherlock->getExp() > 300 && sherlock->getHp() > 335)
                {
                    sherlockbag->insert(robotSW->getKeepItem());
                }
                else
                {
                    if (sherlockbag->find(EXCEMPTION_CARD))
                    {
                        sherlockbag->get(EXCEMPTION_CARD)->use(sherlock, robotSW);
                        sherlockbag->insert(robotSW->getKeepItem());
                    }
                    else
                    {
                        sherlock->setHp(ceil(sherlock->getHp() - sherlock->getHp() * (float)15 / 100));
                        sherlock->setExp(ceil(sherlock->getExp() - sherlock->getExp() * (float)15 / 100));
                    }
                }
            }

            else if (RobotC *robotC = dynamic_cast<RobotC *>(robot))
            {

                if (sherlock->getHp() > 500)
                {
                    sherlockbag->insert(robotC->getKeepItem());
                    isStop();
                }

                else
                {
                    if (sherlockbag->find(EXCEMPTION_CARD))
                    {
                        sherlockbag->get(EXCEMPTION_CARD)->use(sherlock, robot);
                        isStop();
                    }

                    else
                    {
                        sherlockbag->insert(robotC->getKeepItem());
                        criminal->getNextPosition();
                    }
                }
            }

            if (sherlockbag->find(MAGIC_BOOK))
            {
                sherlockbag->get(MAGIC_BOOK)->use(sherlock, nullptr);
            }

            else if (sherlockbag->find(ENERGY_DRINK))
            {
                sherlockbag->get(ENERGY_DRINK)->use(sherlock, nullptr);
            }

            else if (sherlockbag->find(FIRST_AID))
            {
                sherlockbag->get(FIRST_AID)->use(sherlock, nullptr);
            }
        }

        else if (watson)
        {

            if (RobotS *robotS = dynamic_cast<RobotS *>(robot))
            {
                return;
            }

            else if (RobotW *robotW = dynamic_cast<RobotW *>(robot))
            {
                if (watsonbag->find(PASSING_CARD))
                {
                    watsonbag->get(PASSING_CARD)->use(watson, robotW);
                    watsonbag->insert(robotW->getKeepItem());
                }

                else
                {
                    if (watson->getHp() > 350)
                    {
                        watsonbag->insert(robotW->getKeepItem());
                    }

                    else
                        watson->setHp(watson->getHp() - watson->getHp() * (float)5 / 100);
                }
            }

            else if (RobotSW *robotSW = dynamic_cast<RobotSW *>(robot))
            {
                if (watsonbag->find(PASSING_CARD))
                {
                    watsonbag->get(PASSING_CARD)->use(watson, robotSW);
                    watsonbag->insert(robotSW->getKeepItem());
                }
                else
                {
                    if (watson->getExp() > 600 && watson->getHp() > 165)
                    {
                        watsonbag->insert(robotSW->getKeepItem());
                    }
                    else
                    {
                        watson->setHp(ceil(watson->getHp() - watson->getHp() * (float)15 / 100));
                        watson->setExp(ceil(watson->getExp() - watson->getExp() * (float)15 / 100));
                    }
                }
            }

            else if (RobotC *robotC = dynamic_cast<RobotC *>(robot))
            {

                if (watson->getHp() > 500)
                {
                    watsonbag->insert(robotC->getKeepItem());
                    isStop();
                }

                else
                {
                    if (watsonbag->find(EXCEMPTION_CARD))
                    {
                        watsonbag->get(EXCEMPTION_CARD)->use(watson, robotC);
                        criminal->getNextPosition();
                    }

                    else
                    {
                        watsonbag->insert(robotC->getKeepItem());
                        criminal->getNextPosition();
                    }
                }
            }

            if (watsonbag->find(MAGIC_BOOK))
            {
                watsonbag->get(MAGIC_BOOK)->use(watson, nullptr);
            }

            else if (watsonbag->find(ENERGY_DRINK))
            {
                watsonbag->get(ENERGY_DRINK)->use(watson, nullptr);
            }

            else if (watsonbag->find(FIRST_AID))
            {
                watsonbag->get(FIRST_AID)->use(watson, nullptr);
            }
        }
    }

    else if (sherlock->getCurrentPosition().isEqual(watson->getCurrentPosition().getRow(), watson->getCurrentPosition().getCol()))
    {
        if (sherlockbag->find(PASSING_CARD))
            watsonbag->insert(sherlockbag->get(PASSING_CARD));

        else if (watsonbag->find(EXCEMPTION_CARD))
            sherlockbag->insert(watsonbag->get(EXCEMPTION_CARD));
    }
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////