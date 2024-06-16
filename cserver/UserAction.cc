// salad, burger, megaburger的製作函式以及GetNextOrder的strategy還沒有做或是只是暫時能跑的版本
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <utility>
#include "GameController.h"
#include "UserAction.h"

using namespace wfrest;
using namespace ctl;
using namespace std;

GameController& controller = GameController::getInstance("default");

// -- DON'T MODIFY ANY CODE ABOVE THIS LINE -- //
// -- YOU ARE ALLOW TO ADD HEADER FILES UNDER THIS LINE -- //

// TODO: Choose your recipe mode
// you may want to have different strategy for different recipe mode

//const string RecipeMode = "salad";
const string RecipeMode = "salad_cheeseburger";
// const string RecipeMode = "all";


// A template GLOBAL VARIABLE vector to store operations
// Feel free to modify this data structure! (or create your own to use)
queue<string> operations;

// A template map to store the position of each counter
// Question: How do you extend this map to store more than one position for a counter?
// Question2: cutting / normal counter have more than one position, how will you handle it?
const map<Counter, pair<int, int>> counterPosition = {
    { BreadCounter, {0, 1} },
    { CabbageCounter, {8, 4} },
    { CheeseBlockCounter, {0, 9} },
    { TomatoCounter, {8, 7} },
    { RawPattyCounter, {0, 20} },
    { StoveCounter, {0, 19} },
    { PlatesCounter, {3, 20} },
    { TrashCounter, {6, 20} },
    { DeliveryCounter, {1, 20} },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, {8, 15} },
    // There are so many normal counters, this is only one of it
    { NormalCounter, {8, 20} }
};

const map<Counter, string> counterDirection = {
    { BreadCounter, "w" },
    { CabbageCounter, "s" },
    { CheeseBlockCounter, "w" },
    { TomatoCounter, "s" },
    { RawPattyCounter, "w" },
    { StoveCounter, "w" },
    { PlatesCounter, "d" },
    { TrashCounter, "d" },
    { DeliveryCounter, "d" },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, "s" },
    // There are so many normal counters, this is only one of it
    { NormalCounter, "d" }
};

void MovePointToPoint(pair<int, int> from, pair<int, int> to, GameController& controller);
void MovePointToCounter(pair<int, int> fromPoint, Counter toCounter, GameController& controller);
void MoveCounterToCounter(Counter from, Counter to, GameController& controller);
void CutIngredient(string youringredient, GameController& controller);
void MakeSalad(GameController& controller);
void MakeMegaBurger(GameController& controller);
void MakeBurger(GameController& controller);
void MakeCheeseBurger(GameController& controller);
Recipe GetNextOrder();


// Init the game (DO NOT MODIFY THIS FUNCTION)
void UserAction::InitGame() {
    Initialize();
    // Set the response to "ok" when finished initialization
    controller.SetResponse("ok");
}

// Just a cute Initializing function
void UserAction::Initialize() {
    cout << "Initializing the game..." << endl;
    while (!operations.empty()){
        operations.pop();
    }
    MovePointToCounter(make_pair(4,10), TomatoCounter, controller);//7
    operations.push("e");
    MovePointToPoint(make_pair(8,7), make_pair(8,14), controller);//7
    operations.push("s");
    operations.push("e");
    CutIngredient("Tomato", controller);//3
    operations.push("e");
    operations.push("a");
    operations.push("s");
    operations.push("e");
    MovePointToCounter(make_pair(8,13), CabbageCounter, controller);//9
    operations.push("s");
    operations.push("e");
    MovePointToPoint(make_pair(8,4), make_pair(8,14), controller);//10
    operations.push("s");
    operations.push("e");
    CutIngredient("Cabbage", controller);//5
    MovePointToCounter(make_pair(8,14), PlatesCounter, controller);//10
    operations.push("d");
    operations.push("e");
    MovePointToPoint(make_pair(1,20),make_pair(8,14), controller);//13
    operations.push("e");
    operations.push("a");
    operations.push("s");
    operations.push("e");
    MovePointToCounter(make_pair(8,13), DeliveryCounter, controller);//14
    operations.push("d");
    operations.push("d");
    operations.push("d");
    operations.push("d");
    operations.push("e");
}

// Main Function of you game logic
void UserAction::SendOperation() {
    if (operations.empty()){
        Recipe g = GetNextOrder();
        if (g == Salad) MakeSalad(controller);
        else if (g == CheeseBurger) MakeCheeseBurger(controller);
        else if (g == Burger) MakeBurger(controller);
        else if (g == MegaBurger) MakeMegaBurger(controller);
    }
        string s = "";
        s = operations.front();
        operations.pop();
        cout << "Operation: " << s << endl;
    
    if (s == "w") controller.MoveUp();
    if (s == "s") controller.MoveDown();
    if (s == "a") controller.MoveLeft();
    if (s == "d") controller.MoveRight();
    if (s == "e") controller.Interact();
    if (s == "f") controller.InteractSpecial();
}


// -- Moving series functions Below -- //

void MovePointToPoint(pair<int, int> from, pair<int, int> to, GameController& controller) {
    // TODO: Move from one point to another point
    
    if (from.second > to.second){
        int movetimes = from.second - to.second;
        for (int i=1; i<=movetimes; i++){
            operations.push("a");
        }
    }
    else if (from.second < to.second){
        int movetimes = to.second - from.second;
        for (int i=1; i<=movetimes; i++){
            operations.push("d");
        }
    }
    if (from.first > to.first){
        int movetimes = from.first - to.first;
        for (int i=1; i<=movetimes; i++){
            operations.push("w");
        }   
    }
    else if (from.first < to.first){
        int movetimes = to.first - from.first;
        for (int i=1; i<=movetimes; i++){
            operations.push("s");
        }
    }
    
}

void MovePointToCounter(pair<int, int> fromPoint, Counter toCounter, GameController& controller) {
    // TODO: Move from one point to a counter
    pair<int, int> targetPosition = counterPosition.at(toCounter);
    
    if (fromPoint.second > targetPosition.second){
        int movetimes = fromPoint.second - targetPosition.second;
        for (int i=1; i<=movetimes; i++){
            operations.push("a");
        }
    }
    else if (fromPoint.second < targetPosition.second){
        int movetimes = targetPosition.second - fromPoint.second;
        for (int i=1; i<=movetimes; i++){
            operations.push("d");
        }
    }
    if (fromPoint.first > targetPosition.first){
        int movetimes = fromPoint.first - targetPosition.first;
        for (int i=1; i<=movetimes; i++){
            operations.push("w");
        }   
    }
    else if (fromPoint.first < targetPosition.first){
        int movetimes = targetPosition.first - fromPoint.first;
        for (int i=1; i<=movetimes; i++){
            operations.push("s");
        }
    }
}

void MoveCounterToCounter(Counter from, Counter to, GameController& controller) {
    // TODO: Move from one counter to another counter
    pair<int, int> fromPoint = counterPosition.at(from);
    pair<int, int> targetPosition = counterPosition.at(to);
    
    if (fromPoint.second > targetPosition.second){
        int movetimes = fromPoint.second - targetPosition.second;
        for (int i=1; i<=movetimes; i++){
            operations.push("a");
        }
    }
    else if (fromPoint.second < targetPosition.second){
        int movetimes = targetPosition.second - fromPoint.second;
        for (int i=1; i<=movetimes; i++){
            operations.push("d");
        }
    }
    if (fromPoint.first > targetPosition.first){
        int movetimes = fromPoint.first - targetPosition.first;
        for (int i=1; i<=movetimes; i++){
            operations.push("w");
        }   
    }
    else if (fromPoint.first < targetPosition.first){
        int movetimes = targetPosition.first - fromPoint.first;
        for (int i=1; i<=movetimes; i++){
            operations.push("s");
        }
    }
}

// -- Moving series functions Above -- //

// -- Strategy for different Recipe Mode Below -- //

Recipe SaladModeStrategy() {
    // Only salad will be in order in Salad Mode
    return Salad;
}

Recipe SaladCheeseburgerModeStrategy() {
    // TODO: Implement your salad cheeseburger mode strategy here
    // Below is a simple example, feel free to change it
    vector<Order> orderList = controller.GetOrderList();
    if (orderList.empty()) {
        return Salad;
    }
    return orderList[0].recipe;
}

Recipe AllRecipeModeStrategy() {
    vector<Order> orderList = controller.GetOrderList();
    if (orderList.empty()){
        return Salad;
    }
    return orderList[0].recipe;
}

Recipe GetNextOrder() {
    // Return the next order based on the Recipe Mode
    if (RecipeMode == "salad") {
        return SaladModeStrategy();
    } else if (RecipeMode == "salad_cheeseburger") {
        return SaladCheeseburgerModeStrategy();
    } else {
        return AllRecipeModeStrategy();
    }
}

// -- Strategy for different Recipe Mode Above -- //

// -- Miscallaneous functions Below -- //

void CutIngredient(string youringredient, GameController& controller) {
    // TODO: Cut the Ingredient for times
    if (youringredient == "Tomato" || youringredient == "Cheese"){
        for (int i=0; i<3; i++){
            operations.push("f");
        }
    }
    if (youringredient == "Cabbage"){
        for (int i=0; i<5; i++){
            operations.push("f");
        }
    }
}

// -- Miscallaneous functions Above -- //

// -- Pipeline Funtions Below -- //

// You are welcome to change the function prototype
// Like changing the return type or adding more parameters
void MakeSalad(GameController& controller) {
    while (!operations.empty()){
        operations.pop();
    }
    pair<int,int> now = controller.GetPlayerPosition();
    MovePointToCounter(now, TomatoCounter, controller);
    operations.push("e");
    MovePointToPoint(make_pair(8,7), make_pair(8,14), controller);//7
    operations.push("s");
    operations.push("e");
    CutIngredient("Tomato", controller);//3
    operations.push("e");
    operations.push("a");
    operations.push("s");
    operations.push("e");
    MovePointToCounter(make_pair(8,13), CabbageCounter, controller);//9
    operations.push("s");
    operations.push("e");
    MovePointToPoint(make_pair(8,4), make_pair(8,14), controller);//10
    operations.push("s");
    operations.push("e");
    CutIngredient("Cabbage", controller);//5
    MovePointToCounter(make_pair(8,14), PlatesCounter, controller);//10
    operations.push("d");
    operations.push("e");
    MovePointToPoint(make_pair(1,20),make_pair(8,14), controller);//13
    operations.push("e");
    operations.push("a");
    operations.push("s");
    operations.push("e");
    MovePointToCounter(make_pair(8,13), DeliveryCounter, controller);//14
    operations.push("d");
    operations.push("e");
}

void MakeBurger(GameController& controller) {
    operations.push("e");
}
void MakeCheeseBurger(GameController& controller) {
    while(!operations.empty()){
        operations.pop();
    }
    pair<int,int> now = controller.GetPlayerPosition();
    MovePointToCounter(now, RawPattyCounter , controller);
    operations.push("e");
    operations.push("a");
    operations.push("w");
    operations.push("e");
    MovePointToPoint(make_pair(0,19), make_pair(2,20), controller);
    operations.push("d");
    operations.push("e");
    MovePointToPoint(make_pair(2,20), make_pair(0,13), controller);
    operations.push("e");
    MovePointToPoint(make_pair(0,13), make_pair(0,10), controller);
    operations.push("w");
    operations.push("e");
    MovePointToPoint(make_pair(0,10), make_pair(0,14), controller);
    operations.push("w");
    operations.push("e");
    CutIngredient("Cheese",controller);
    operations.push("e");
    operations.push("a");
    operations.push("w");
    operations.push("e");
    MovePointToPoint(make_pair(0,13), make_pair(0,1),controller);
    operations.push("w");
    operations.push("e");
    MovePointToPoint(make_pair(0,1),make_pair(0,13),controller);
    operations.push("w");
    operations.push("e");
    operations.push("e");
    MovePointToPoint(make_pair(0,13),make_pair(0,19),controller);
    operations.push("w");
    operations.push("e");
    MovePointToPoint(make_pair(0,19), make_pair(1,20),controller);
    operations.push("d");
    operations.push("e");
}
void MakeMegaBurger(GameController& controller) {
    operations.push("e");
}


