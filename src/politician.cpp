// Local headers
#include <politician.hpp>

using std::string;
using std::move;

politician_core::politician_core(string name, string party)
	: name(move(name)), party(move(party))
{}

politician_update::politician_update(string name, string party, string new_party)
	: politician_core(move(name), move(party)), new_party(move(new_party))
{}

politician::politician(string name, string party, string info, const short points)
	: politician_core(move(name), move(party)), info(move(info)), points(points)
{}

rating::rating(
		string name, string party, string description, const short points,
		string date_time)
	: politician_core(move(name), move(party)),
		description(move(description)), points(points), date_time(move(date_time))
{}

void politician_core::print_data() const
{
	std::cout << "Name: " << name << "\n"
	             "Party: " << party << "\n";
}

void politician_update::print_data() const
{
	politician_core::print_data();
	std::cout << "New party: " << new_party << "\n";
}

void politician::print_data() const
{
	politician_core::print_data();
	std::cout << "Rating points: " << points << "\n"
	             "Information: " << info << "\n";
}

void rating::print_data() const
{
	politician_core::print_data();
	std::cout << "Points: " << points << "\n";
	if(date_time.empty() == false)
		std::cout << "Date/time: " << date_time << "\n";
	std::cout << "Description: " << description << "\n";
}

