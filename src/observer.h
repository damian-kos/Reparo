#pragma once
#include <iostream>
#include <list>
#include <string>
#include <vector>

class IObserver {
public:
  virtual ~IObserver() {};
  virtual void Update() = 0;

};

class ISubject {
public:
  virtual ~ISubject() {};
  virtual void Attach(IObserver* observer) = 0;
  virtual void Detach(IObserver* observer) = 0;
  virtual void Notify() = 0;
};

class Subject : public ISubject {
private:
  std::vector<IObserver*> observers;
public:
  void Attach(IObserver* observer) override {
    for (auto& obs : observers) {
      if (obs == observer) {
        std::cout << "Observer is already attached!" << std::endl;
        return;
      }
    }
    observers.push_back(observer);
    std::cout << "Attach: " << observer << " | Observers count: " << observers.size() << std::endl;
  }

  void Detach(IObserver* observer) override {
    std::cout << "Detach: " << observer << " | Before detach count: " << observers.size() << std::endl;
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    std::cout << "Detach: " << observer << " | After detach count: " << observers.size() << std::endl;
  }

  void Notify() override {
    for (IObserver* observer : observers) {
      std::cout << "Observers notified" << std::endl;
      observer->Update();
    }
  }
};