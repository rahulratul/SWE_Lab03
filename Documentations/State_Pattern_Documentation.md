# Lab 03 -- State Pattern

## 1. Pattern Name
**State Pattern**

## 2. Category
**Behavioral Design Pattern**

## 3. Intent
The State pattern lets an object change how it behaves when its internal condition changes, so that from the outside it looks like the object switched to a completely different class. Instead of writing one big block of conditional logic to handle every situation, each situation (state) gets its own small class, and the main object simply hands off the work to whichever state is currently active.

## 4. Problem Statement
A lot of real objects act differently depending on what mode they are currently in. If that behavior is hard-coded using if-else or switch statements inside one class, the class quickly turns into a tangled mess that is hard to read and even harder to test. Every time a new mode needs to be added, a developer has to go back and update every single conditional block that checks the mode, which is slow and easy to get wrong. This also breaks the Open/Closed Principle, since existing code has to be modified instead of just extended.

## 5. Motivation
Take a washing machine as an example. It can be Idle, Washing, or Paused. A quick-and-dirty implementation would keep the state as a plain string or enum inside the WashingMachine class and check it everywhere:
```cpp
if (state == "IDLE") { ... }
else if (state == "WASHING") { ... }
else if (state == "PAUSED") { ... }
```
As more states and buttons are added (spin, rinse, add a delay timer), these checks get copy-pasted into every method that cares about the current mode. The State pattern fixes this by pulling each branch out into its own class. The WashingMachine class then just forwards the request to whichever state object is active at the time, without needing to know the details.

## 6. Pattern Structure (UML Class Diagram)
> **UML diagram:** Add the State Pattern UML image pushed by the team to the repository and update the filename below.
> 
> `![State Pattern UML](../State%20Pattern/state_uml.jpg)`

## 7. Class Responsibilities
* **WashingMachine (Context)** – Keeps a reference to the current `CycleState` object and passes user actions (`startCycle`, `pauseCycle`) on to it. It does not contain any state-specific logic itself.
* **CycleState (Interface)** – Declares the two operations every state must be able to handle: `startCycle()` and `pauseCycle()`.
* **IdleState** – Handles behavior while the machine is switched off. Starting the cycle moves it to `WashingState`; pausing does nothing since there is nothing running yet.
* **WashingState** – Handles behavior while the machine is actively washing. Pausing moves it to `PausedState`; pressing start again has no effect.
* **PausedState** – Handles behavior while the machine is paused mid-cycle. Pressing start resumes washing and moves back to `WashingState`; pausing again has no effect.

## 8. Code Implementation
```cpp
#include <iostream>
using namespace std;

class WashingMachine; // forward declaration

// State interface
class CycleState {
public:
    virtual void startCycle(WashingMachine* machine) = 0;
    virtual void pauseCycle(WashingMachine* machine) = 0;
    virtual ~CycleState() {}
};

// Context
class WashingMachine {
private:
    CycleState* state;
public:
    void setState(CycleState* s) { state = s; }
    void startCycle() { state->startCycle(this); }
    void pauseCycle() { state->pauseCycle(this); }
};

// Concrete states (declared here, defined once WashingMachine is visible)
class IdleState : public CycleState {
public:
    void startCycle(WashingMachine* machine) override;
    void pauseCycle(WashingMachine* machine) override;
};

class WashingState : public CycleState {
public:
    void startCycle(WashingMachine* machine) override;
    void pauseCycle(WashingMachine* machine) override;
};

class PausedState : public CycleState {
public:
    void startCycle(WashingMachine* machine) override;
    void pauseCycle(WashingMachine* machine) override;
};

void IdleState::startCycle(WashingMachine* machine) {
    cout << "Wash cycle started.\n";
    machine->setState(new WashingState());
}

void IdleState::pauseCycle(WashingMachine* machine) {
    cout << "Nothing to pause, the machine is idle.\n";
}

void WashingState::startCycle(WashingMachine* machine) {
    cout << "Already washing.\n";
}

void WashingState::pauseCycle(WashingMachine* machine) {
    cout << "Pausing the wash cycle...\n";
    machine->setState(new PausedState());
}

void PausedState::startCycle(WashingMachine* machine) {
    cout << "Resuming the wash cycle...\n";
    machine->setState(new WashingState());
}

void PausedState::pauseCycle(WashingMachine* machine) {
    cout << "Already paused.\n";
}

int main() {
    WashingMachine machine;
    machine.setState(new IdleState());

    machine.startCycle(); // Wash cycle started.
    machine.pauseCycle(); // Pausing the wash cycle...
    machine.startCycle(); // Resuming the wash cycle...
    machine.pauseCycle(); // Pausing the wash cycle...
    machine.pauseCycle(); // Already paused.

    return 0;
}
```

## 9. Execution Flow (Object Interaction)
1. A `WashingMachine` object is created and given an `IdleState` object as its starting state.
2. The client calls `machine.startCycle()`, and the Context forwards this to `state->startCycle(this)`.
3. `IdleState::startCycle()` prints a message and calls `machine->setState(new WashingState())` — the Context now points to `WashingState`.
4. The client calls `machine.pauseCycle()`, which is forwarded to `WashingState::pauseCycle()`, switching the Context to `PausedState`.
5. The client calls `machine.startCycle()` again, which is forwarded to `PausedState::startCycle()`, resuming the wash and switching back to `WashingState`.
6. Each state class only ever decides its own next state; `WashingMachine` never contains an if-statement asking 'which state am I in right now'.

## 10. Advantages
* Removes long conditional chains from the Context class.
* Each state's behavior and its transitions are isolated, so the code is easier to follow.
* New states can be added without touching the existing state classes (Open/Closed Principle).
* Invalid actions for a given state (like pausing an idle machine) are handled explicitly and cleanly.

## 11. Limitations
* Adds more classes to the project, which can feel like overkill when there are only one or two states.
* Because the transition logic is spread across several classes, it can be harder to see the full picture without a diagram.
* If not designed carefully, state classes may end up needing to know about each other, which increases coupling.

## 12. Real-life Applications
* Washing machines and similar home appliances (Idle / Washing / Spinning / Paused).
* Traffic light controllers (Red / Yellow / Green).
* Order tracking systems (Placed → Shipped → Delivered).
* ATMs (Idle, CardInserted, PinEntered, Dispensing).
* Network connection handling (Closed, Listening, Established).
* Game character behavior (Idle, Walking, Jumping, Attacking).

## 13. Industry Examples
* The Android Activity lifecycle (Created, Started, Resumed, Paused, Destroyed) follows the same idea.
* Streaming apps such as Netflix use a similar approach for Buffering, Playing, Paused, and Error states.
* Ticketing tools such as JIRA model a ticket's status (To-Do → In Progress → Done) as states.
* Loan or application approval pipelines in banking software are often built around state transitions.

## 14. Conclusion
The State pattern turns behavior that depends on the current mode into a set of small, swappable classes, keeping the main object free of complicated conditionals. Because each state manages its own transitions, the washing machine example stays easy to extend — adding a new mode such as Spinning only means writing one new class, without touching `WashingMachine`, `IdleState`, `WashingState`, or `PausedState`.
