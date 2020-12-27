// Copyright (c) 2020 DGB. All rights reserved.

#include <iomanip>
#include <conio.h>
#include <iostream>
#include <algorithm>

#include "ContactBook.h"


namespace cb
{
	inline void Trim(std::string& str)
	{
		size_t startpos = str.find_first_not_of(' ');
		str = (startpos == std::string::npos) ? "" : str.substr(startpos);
		size_t endpos = str.find_last_not_of(' ');
		str = (endpos == std::string::npos) ? "" : str.substr(0, endpos + 1);
	}

	void ContactBook::ShowContact() const
	{
		if (contacts.empty())
		{
			std::cout << "There are no contacts in the contact book."
				<< std::endl << std::endl;
			return;
		}

		for (const auto& i : contacts)
			i.Print();
	}

	void ContactBook::AddContact()
	{
		contacts.push_back(CreateContact());
		std::sort(contacts.begin(), contacts.end(),
			[](const Contact& c1, const Contact& c2) { return c1.name < c2.name; });
	}

	void ContactBook::FindContact(contact_action_t action)
	{
		std::string findString;
		size_t index;
		int key = 0;
		int count = 0;
		std::cout << "Find:";
		while (true)
		{
			key = _getch();

			if (key == static_cast<int>(Keys::ESCAPE))
				break;
			else if (key == static_cast<int>(Keys::ENTER) and count == 1)
			{
				action(index);
				break;
			}
			else if (key == static_cast<int>(Keys::BACKSPACE) and !findString.empty())
				findString.erase(findString.begin() + findString.size() - 1);
			else
				findString += key;

			system("cls");
			std::cout << "Find: ";

			if (findString.empty())
				continue;

			count = 0;
			std::cout << findString;
			for (size_t i = 0; i < contacts.size(); ++i)
			{
				size_t pos = contacts[i].name.find(findString);
				if (pos != std::string::npos and pos == 0)
				{
					std::cout << std::endl;
					contacts[i].Print();
					index = i;
					count++;
				}

			}

		}
		std::cout << std::endl;
	}

	void ContactBook::RedactContact()
	{
		contact_action_t redact = [this](size_t index)
		{
			system("cls");
			std::cout << "Redact contact: " << std::endl;
			contacts[index] = CreateContact();
		};
		FindContact(redact);
	}

	void ContactBook::DeleteContact()
	{
		contact_action_t del = [this](size_t index)
		{
			contacts.erase(contacts.begin() + index);
		};
		FindContact(del);
	}

	ContactBook::ContactBook() : file{ "contacts" }
	{
		std::ifstream in(file);
		if (in.bad())
		{
			in.close();
			return;
		}
		Contact c;
		while (in >> c)
			contacts.push_back(c);
		in.close();
	}

	ContactBook::~ContactBook()
	{
		std::ofstream out(file);
		if (out.good())
			for (const auto& i : contacts)
				out << i;
		out.close();
	}

	Contact ContactBook::CreateContact()
	{
		std::string tmp;
		Contact c;

		std::cout << "Name:" << std::endl;
		std::getline(std::cin, tmp);
		Trim(tmp);
		c.name = tmp;

		std::cout << "Phone number:" << std::endl;
		std::getline(std::cin, tmp);
		Trim(tmp);
		c.phone = tmp;

		std::cout << "Age:" << std::endl;
		std::getline(std::cin, tmp);
		Trim(tmp);
		c.age = std::stoi(tmp);

		std::cout << "Description:" << std::endl;
		std::getline(std::cin, tmp);
		Trim(tmp);
		c.description = tmp;

		return c;
	}

	std::ostream& operator<<(std::ostream& os, const Contact& c)
	{
		os << c.name << std::endl;
		os << c.phone << std::endl;
		os << c.age << std::endl;
		os << c.description << std::endl;
		return os;
	}

	std::istream& operator>>(std::istream& os, Contact& c)
	{
		os >> c.name;
		os >> c.phone;
		os >> c.age;
		os >> c.description;
		return os;
	}

	bool operator==(const Contact& c1, const Contact& c2)
	{
		if (c1.name == c2.name
			and c1.phone == c2.phone
			and c1.age == c2.age
			and c1.description == c2.description)
		{
			return true;
		}
		return false;
	}

	void Contact::Print() const
	{
		std::cout << "Name:         " << std::setw(20) << name << std::endl;
		std::cout << "Phone number: " << std::setw(20) << phone << std::endl;
		std::cout << "Age:          " << std::setw(20);
		age ? std::cout << age : std::cout << '-';
		std::cout << std::endl;
		std::cout << "Description:  " << std::setw(20) << description << std::endl;
		std::cout << "==================================" << std::endl;
	}
}