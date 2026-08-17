# Lab 03 -- Strategy Pattern

## 1. Pattern Name
**Strategy Pattern**

## 2. Category
**Behavioral Design Pattern**

## 3. Intent
The Strategy pattern packages a group of related algorithms into separate classes and makes them interchangeable at runtime, without the calling code needing to change. The client decides which behavior to plug in, and the class that uses it never needs to know how that behavior is actually carried out.

## 4. Problem Statement
Many systems need more than one way to perform the same task — for instance, several ways to notify a user, sort a list, or calculate a route. If every option is written as a conditional branch inside one method, that method keeps growing every time a new option shows up, and unrelated pieces of logic end up crammed together in a single block, making the whole thing harder to test and maintain.

## 5. Motivation
Consider a system that needs to notify users through different channels: Email, SMS, or Push notifications. A quick implementation would check the channel type directly inside the notify method:
```cpp
if (channel == "email") { sendEmail(); }
else if (channel == "sms") { sendSMS(); }
else if (channel == "push") { sendPush(); }
```
Adding a new channel later, say WhatsApp, means going back into this method and risking breaking the channels that already work — a clear violation of the Open/Closed Principle. The Strategy pattern avoids this by giving each channel its own class that implements a shared interface, so the `NotificationService` class never has to change when a new channel is added.

## 6. Pattern Structure (UML Class Diagram)
> **UML diagram:** Add the Strategy Pattern UML image pushed by the team to the repository and update the filename below.
> 
> `![Strategy Pattern UML](../Strategy%20Pattern/Strategy-UML-NotificationService.png)`

## 7. Class Responsibilities
* **NotificationStrategy (Interface)** – Declares the `send(message)` operation that every concrete channel must implement.
* **EmailNotification** – Sends the message through email.
* **SMSNotification** – Sends the message through SMS.
* **PushNotification** – Sends the message as a push notification.
* **NotificationService (Context)** – Holds a reference to a `NotificationStrategy` and passes the message to it, without knowing which channel is actually being used.

## 8. Code Implementation
```cpp
#include <iostream>
using namespace std;

// Strategy interface
class NotificationStrategy {
public:
    virtual void send(string message) = 0;
    virtual ~NotificationStrategy() {}
};

// Concrete strategies
class EmailNotification : public NotificationStrategy {
public:
    void send(string message) override {
        cout << "Email sent: " << message << "\n";
    }
};

class SMSNotification : public NotificationStrategy {
public:
    void send(string message) override {
        cout << "SMS sent: " << message << "\n";
    }
};

class PushNotification : public NotificationStrategy {
public:
    void send(string message) override {
        cout << "Push notification sent: " << message << "\n";
    }
};

// Context
class NotificationService {
private:
    NotificationStrategy* strategy;
public:
    void setStrategy(NotificationStrategy* s) { strategy = s; }
    void notify(string message) { strategy->send(message); }
};

int main() {
    NotificationService service;
    EmailNotification email;
    service.setStrategy(&email);
    service.notify("Your order has been confirmed.");

    SMSNotification sms;
    service.setStrategy(&sms);
    service.notify("Your OTP is 4821.");

    PushNotification push;
    service.setStrategy(&push);
    service.notify("Your package is out for delivery.");

    return 0;
}
```

## 9. Execution Flow (Object Interaction)
1. The client creates a `NotificationService` and a concrete strategy object, e.g. `EmailNotification`.
2. The client calls `service.setStrategy(&email)` to plug the chosen strategy into the Context.
3. The client calls `service.notify("Your order has been confirmed.")`.
4. `NotificationService` forwards this to `strategy->send(message)` without knowing how the message is actually delivered.
5. To switch channels, the client just assigns a different strategy object and calls `notify()` again — nothing inside `NotificationService` needs to change.

## 10. Advantages
* Takes the algorithm-selection logic completely out of the Context class.
* New behaviors (channels) can be added as new classes without editing existing code.
* Each behavior can be built, tested, and reused on its own.
* Follows the Open/Closed Principle and allows the behavior to be swapped at runtime.

## 11. Limitations
* Adds extra classes for what might be a fairly simple decision.
* The client has to know about the available strategies in order to pick the right one.
* For a small, fixed number of options, the pattern can feel like more structure than the problem actually needs.

## 12. Real-life Applications
* Notification systems that send alerts through email, SMS, or push (as in the example above).
* Payment method selection in e-commerce checkout flows.
* Route or navigation options in map applications — fastest, shortest, avoid tolls.
* Sorting algorithms chosen at runtime through a custom comparator.
* Compression algorithm selection in archiving tools.
* Authentication options such as logging in with Google, Facebook, or a plain email/password form.

## 13. Industry Examples
* Java's `Collections.sort()` accepts a `Comparator` object, which is a textbook use of the Strategy pattern.
* Spring Framework applications commonly inject different strategy implementations through dependency injection.
* Firebase and similar platforms let apps switch between multiple notification channels using an interchangeable-strategy style setup.
* Google Maps lets users switch between driving, walking, transit, and cycling as swappable routing strategies.

## 14. Conclusion
The Strategy pattern separates "what to do" from "how it gets done," which gives the `NotificationService` example the freedom to support any number of channels without ever growing a tangled conditional block. This keeps the system easy to extend, test, and maintain as new notification channels are introduced over time.
