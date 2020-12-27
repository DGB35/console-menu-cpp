// Copyright (c) 2020 DGB. All rights reserved.

#include "Menu.h"

namespace menu
{
    Menu::Menu(const HANDLE& hConsole_, const std::string& name_) :
        hConsole{ hConsole_ }, menuName{ name_ }, tooltip{ }, position{ 0 }
    {
        ShowCursor(false);
    }

    void Menu::AddItem(const std::string& name, action_t action)
    {
        menuItems.push_back(MenuItem(name, action));
    }

    void Menu::GetInput()
    {
        if (menuItems.empty())
            return;
        system("cls");
        Initialize();
        SelectItem(0);
        timer.InitializeThread(*this);
        int key = 0;
        while (key != static_cast<int>(Keys::ESCAPE))
        {
            key = _getch();
            if (key == static_cast<int>(Keys::ENTER))
            {
                {
                    std::lock_guard <std::mutex> guard(lock);
                    system("cls");
                    ShowCursor(true);
                    menuItems[position].action();
                    system("pause");
                    system("cls");
                    ShowCursor(false);
                    ShowMenu();
                }
                SelectItem(position);
            }
            if (key != static_cast<int>(Keys::ARROW))
                continue;
            key = _getch();
            const auto selected = position;

            if (key == static_cast<int>(Keys::ARROW_UP))        ++position;
            else if (key == static_cast<int>(Keys::ARROW_DOWN)) --position;

            if (position < 0)
                position = menuItems.size() - 1;
            else if (position > menuItems.size() - 1)
                position = 0;

            if (selected != position)
                SelectItem(position);
        }
    }

    void Menu::ChangeName(const std::string& name)
    {
        menuName = name;
    }

    void Menu::Initialize()
    {
        ShowTime();
        ShowTooltip();
        CONSOLE_SCREEN_BUFFER_INFO screenBuffer = { 0 };
        for (auto& item : menuItems)
        {
            GetConsoleScreenBufferInfo(hConsole, &screenBuffer);
            item.position = screenBuffer.dwCursorPosition;
            std::cout << item.name << std::endl;
        }
    }

    void Menu::SelectItem(int position)
    {
        std::lock_guard <std::mutex> guard(lock);
        system("color 0F");
        SetConsoleCursorPosition(hConsole, menuItems[position].position);
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        std::cout << menuItems[position].name;
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
        SetCursorToEnd();
    }

    void Menu::SetCursorToEnd()
    {
        SetConsoleCursorPosition(hConsole, menuItems[menuItems.size() - 1].position);
        std::cout << std::endl;
    }

    void Menu::ShowMenu() const
    {
        ShowTime();
        ShowTooltip();
        ShowItems();
    }

    void Menu::ShowTime() const
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond);
        std::cout << std::endl;
    }

    void Menu::ShowTooltip() const
    {
        std::cout << "Esc - exit" << std::endl << std::endl << menuName << std::endl;
    }

    void Menu::ShowItems() const
    {
        for (auto& item : menuItems)
            std::cout << item.name << std::endl;
    }

    void Menu::ShowCursor(bool visibility)
    {
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(hConsole, &structCursorInfo);
        structCursorInfo.bVisible = visibility;
        SetConsoleCursorInfo(hConsole, &structCursorInfo);
    }

    void Menu::Timer::InitializeThread(Menu& menu)
    {
        isWorking = true;
        lastCheckedTime = std::chrono::system_clock::now() + std::chrono::seconds{ 1 };
        timeThread = std::thread{ [&menu, this]() { menu.CheckTime(*this); } };
    }

    void Menu::CheckTime(Timer &timer)
    {
        COORD timePosition{ 0,0 };
        while (timer.isWorking)
        {
            if (timer.lastCheckedTime <= std::chrono::system_clock::now())
            {
                std::lock_guard <std::mutex> guard(lock);
                SetConsoleCursorPosition(hConsole, timePosition);
                ShowTime();
                SetCursorToEnd();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
        }
    }

    Menu::~Menu()
    {
        timer.isWorking = false;
        if (timer.timeThread.joinable())
            timer.timeThread.join();
    }
}


