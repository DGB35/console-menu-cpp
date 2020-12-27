#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <functional>

using age_t = unsigned int;
using contact_action_t = std::function<void(size_t)>;
enum class Keys { ARROW = 224, ARROW_UP = 80, ARROW_DOWN = 72, ENTER = 13, ESCAPE = 27, BACKSPACE = 8 };

namespace cb
{
	struct Contact
	{
		Contact()
			: name{ "-" }, phone{ "-" }, age{ 0 }, description{ "-" }
		{
		}

		std::string name;
		std::string phone;
		age_t age;
		std::string description;

		void Print() const;
		friend std::ostream& operator<<(std::ostream& os, const Contact& c);
		friend std::istream& operator>>(std::istream& os, Contact& c);
		friend bool operator==(const Contact& c1, const Contact& c2);
	};

	class ContactBook
	{
	public:
		void ShowContact() const;
		void AddContact();
		void FindContact(contact_action_t action = [](size_t) {});
		void RedactContact();
		void DeleteContact();
	public:
		ContactBook();
		~ContactBook();
	private:
		Contact CreateContact();
	private:
		std::vector<Contact> contacts;
		std::string file;
	};

}