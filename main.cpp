/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 01.03.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.

#include "study_in_pink2.h"

using namespace std;

class TestStudyInPink {
public:
    TestStudyInPink() = default;
    
    // static void tc_1001() {
    //     ::tc_1001();
    // }
    // static void tc_1004() {
    //     ::tc_1004();
    // }
    // static void tc_1009() {
    //     ::tc_1009();
    // }
    // static void tc_1014() {
    //     ::tc_1014();
    // }
    // static void tc_1015() {
    //     ::tc_1015();
    // }
    // static void tc_1016() {
    //     ::tc_1016();
    // }
    // static void tc_1017() {
    //     ::tc_1017();
    // }
    // static void tc_1043() {
    //     cout << "----- Testcase 43 -----" << endl;
    //     int num_walls = 0;
    //     Position arr_walls[] = {};
    //     int num_fake_walls = 1;
    //     Position arr_fake_walls[] = {Position(5, 5)};
    //     Map * map = new Map(10, 10, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
    //     for (int i = 0; i < map->num_rows; i++) {
    //         for (int j = 0; j < map->num_cols; j++) {
    //             cout << map->map[i][j]->getType();
    //             if (map->map[i][j]->getType() == FAKE_WALL) {
    //                 FakeWall *fakewall = dynamic_cast<FakeWall*>(map->map[i][j]);
    //                 cout << fakewall->getReqExp();
    //             }
    //             cout << "-";
    //         }
    //         cout << endl;    
    //     }
    //     delete map;
    // }
    // static void tc_1057() {
    //     ::tc_1057();
    // }
    // static void tc_1062() {
    //     ::tc_1062();
    // }
    // static void tc_1063() {
    //     ::tc_1063();
    // }
    // static void tc_1066() {
    //     ::tc_1066();
    // }
    // static void tc_1067() {
    //     ::tc_1067();
    // }
    // static void tc_1070() {
    //     ::tc_1070;
    // }
    // static void tc_1094() {
    //     ::tc_1094();
    // }
    // static void tc_1099() {
    //     ::tc_1099;
    // }
    // static void tc_1109() {
    //     ::tc_1109;
    // }
    // static void tc_1114() {
    //     ::tc_1114;
    // }
    // static void tc_1120() {
    //     ::tc_1120;
    // }
    // static void tc_1132() {
    //     ::tc_1132;
    // }
    // static void tc_1138() {
    //     ::tc_1138;
    // }
    // static void tc_1143() {
    //     ::tc_1143;
    // }
    // static void tc_1153() {
    //     ::tc_1153;
    // }
    // static void tc_1159() {
    //     ::tc_1159;
    // }
    // static void tc_1167() {
    //     ::tc_1167;
    // }
    // static void tc_1171() {
    //     ::tc_1171;
    // }
    // static void tc_1181() {
    //     ::tc_1181;
    // }
    // static void tc_1185() {
    //     ::tc_1185;
    // }
    // static void tc_1199() {
    //     ::tc_1199;
    // }
    // static void tc_1211() {
    //     ::tc_1211;
    // }
    // static void tc_1223() {
    //     ::tc_1223;
    // }
    // static void tc_1235() {
    //     ::tc_1235;
    // }
    // static void tc_1247() {
    //     ::tc_1247;
    // }
    static void tc_1278() {
        cout << "----- Testcase 278 -----" << endl;
        int num_walls = 4;
        Position arr_walls[] = {Position(1, 2), Position(2, 3), Position(3, 4), Position(4,5)};
        int num_fake_walls = 2;
        Position arr_fake_walls[] = {Position(2, 0), Position(4,0)};
    
        Map * map = new Map(10, 10, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
            
        Sherlock * sherlock = new Sherlock(1, "RUU", Position(1, 3), map, 250, 450);
    
        Watson * watson = new Watson(2, "LU", Position(2, 1), map, 300, 350);
    
        Criminal * criminal = new Criminal(0, Position(7, 9), map, sherlock, watson);
    
        BaseItem * excemptionCard = new ExcemptionCard();
    
        cout << excemptionCard->canUse(criminal, NULL);
    
        delete map;
        delete sherlock;
        delete watson;
        delete criminal;
        delete excemptionCard;
    }
    
    // static void tc_1297() {
    //     ::tc_1297;
    // }
    // static void tc_1323() {
    //     ::tc_1323;
    // }
    static void tc_1323() {
    cout << "----- Testcase 323 -----" << endl;
    int num_walls = 4;
    Position arr_walls[] = {Position(1, 2), Position(2, 3), Position(3, 4), Position(4,5)};
    int num_fake_walls = 2;
    Position arr_fake_walls[] = {Position(2, 0), Position(4,0)};

    Map * map = new Map(10, 10, num_walls, arr_walls, num_fake_walls, arr_fake_walls);

    Watson * watson = new Watson(1, "RUU", Position(1, 3), map, 150, 450);

    BaseBag * watsonBag = new WatsonBag(watson);

    cout << watsonBag->str() << endl;

    PassingCard *passingCard = new PassingCard("all");
    watsonBag->insert(passingCard);
    cout << watsonBag->str() << endl;
    
    watsonBag->get(PASSING_CARD);
    cout << watsonBag->str() << endl;

    delete map;
    delete watson;
}

    static void tc_1286() {
        cout << "----- Testcase 286 -----" << endl;
        int num_walls = 4;
        Position arr_walls[] = {Position(1, 2), Position(2, 3), Position(3, 4), Position(4,5)};
        int num_fake_walls = 2;
        Position arr_fake_walls[] = {Position(2, 0), Position(4,0)};
    
        Map * map = new Map(10, 10, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
            
        Sherlock * sherlock = new Sherlock(1, "RUU", Position(1, 3), map, 250, 450);
    
        Watson * watson = new Watson(2, "LU", Position(2, 1), map, 300, 350);
    
        Criminal * criminal = new Criminal(0, Position(7, 9), map, sherlock, watson);
    
        BaseItem * passingCard = new PassingCard("all");
    
        cout << passingCard->canUse(criminal, NULL);
    
        delete map;
        delete sherlock;
        delete watson;
        delete criminal;
        delete passingCard;
    }
    static void tc_1194() {
        cout << "----- Testcase 194 -----" << endl;

        Configuration * config = new Configuration("185.txt");
        cout << "Map Num Rows: " << config->map_num_rows << endl;
        cout << "Map Num Cols: " << config->map_num_cols << endl;
        cout << "Max Num Moving Objects: " << config->max_num_moving_objects << endl;
        cout << "Num Wall: " << config->num_walls << endl;
        if (config->num_walls != 0) {
            cout << "Array Wall: " << config->arr_walls->str() << endl;
        }
        cout << "Num Fake Wall: " << config->num_fake_walls << endl;
        if (config->num_fake_walls != 0) {
            cout << "Array Fake Wall: " << config->arr_fake_walls->str() << endl;
        }
        cout << "Shelock Moving Rule: " << config->sherlock_moving_rule << endl;
        cout << "Sherlock init position: " << config->sherlock_init_pos.str() << endl;
        cout << "Sherlock init hp: " << config->sherlock_init_hp << endl;
        cout << "Sherlock init exp: " << config->sherlock_init_exp << endl;
        cout << "Watson Moving Rule: " << config->watson_moving_rule << endl;
        cout << "Watson init position: " << config->watson_init_pos.str() << endl;
        cout << "Watson init hp: " << config->watson_init_hp << endl;
        cout << "Watson init exp: " << config->watson_init_exp << endl;
        cout << "Criminal init position: " << config->criminal_init_pos.str() << endl;
        cout << "Number step: " << config->num_steps << endl;
        delete config;
    }
    static void tc_1253() {
        cout << "----- Testcase 253 -----" << endl;
        BaseItem * magicBook = new MagicBook();
        
        delete magicBook;
    }
    static void tc_1235() {
    cout << "----- Testcase 235 -----" << endl;
    int num_walls = 4;
    Position arr_walls[] = {Position(7, 0), Position(6, 1), Position(8, 1), Position(7,2)};
    int num_fake_walls = 1;
    Position arr_fake_walls[] = {Position(2, 0)};

    Map * map = new Map(10, 10, num_walls, arr_walls, num_fake_walls, arr_fake_walls);

    Sherlock * sherlock = new Sherlock(1, "RUU", Position(1, 3), map, 250, 450);
    Watson * watson = new Watson(2, "LU", Position(2, 1), map, 300, 350);

    Criminal * criminal = new Criminal(0, Position(7, 9), map, sherlock, watson);
    RobotC * robotC = new RobotC(3, Position(7,1), map, criminal);

    cout << "First position: " << robotC->getCurrentPosition().str() << endl;
    Position new_position = robotC->getNextPosition();
    RobotC * robotC_new = new RobotC(4, new_position, map, criminal);

    cout << "Next position: " << robotC_new->getCurrentPosition().str() << endl;

    delete robotC;
    delete robotC_new;
    delete sherlock;
    delete watson;
    delete criminal;
    delete map;
}
};

int main(int argc, const char *argv[])
{

    TestStudyInPink::tc_1235();

    return 0;
}

// // g++ -o main main.cpp study_in_pink2.cpp -I . -std=c++11;./main