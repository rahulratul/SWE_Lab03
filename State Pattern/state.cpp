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