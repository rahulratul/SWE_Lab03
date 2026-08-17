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
