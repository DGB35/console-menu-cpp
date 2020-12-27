// Copyright (c) 2020 DGB. All rights reserved.

#include <Windows.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>
#include <mutex>

using action_t = std::function<void()>;

namespace menu
{
    struct MenuItem
    {
        MenuItem(std::string name_, action_t action_, COORD position_ = COORD{}) 
            : name{ name_ }, action{ action_ }, position{ position_ }
        {
        }
        std::string name;
        action_t action;
        COORD position;
    };

    class Menu
    {
    public:
        Menu(const HANDLE&, const std::string& name = "Menu:");
        ~Menu();
    public:
        void ChangeName(const std::string&);
        void AddItem(const std::string&, action_t);
        void GetInput();
    private:
        void Initialize();
        void SelectItem(int position);
        void SetCursorToEnd();
        void ShowMenu() const;
        void ShowTime() const;
        void ShowTooltip() const;
        void ShowItems() const;
        void ShowCursor(bool);
    private:
        Menu(const Menu&) = delete;
        Menu& operator = (const Menu&) = delete;
    private:
        enum class Keys { ARROW = 224, ARROW_UP = 80, ARROW_DOWN = 72, ENTER = 13, ESCAPE = 27 };
        HANDLE hConsole;
        int position;
        std::string menuName;
        std::string tooltip;
        std::vector<MenuItem> menuItems;
    private:
        struct Timer
        {
            Timer() : isWorking{ false } {}
            void InitializeThread(Menu& menu);
            bool isWorking;
            std::chrono::system_clock::time_point lastCheckedTime;
            std::thread timeThread;
        };
    private:
        void CheckTime(Timer&);
        Timer timer;
        std::mutex lock;
    };
}