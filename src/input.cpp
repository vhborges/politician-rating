// Local headers
#include <input.hpp>
#include <politician.hpp>
#include <database.hpp>

// Command line parser
#include <CLI11.hpp>

// Allows Unicode strings' manipulation
#include <boost/locale.hpp>

using std::string;
using std::move;
using boost::locale::to_upper;

bool confirm_operation(const string& operation)
{
	string confirm;
	std::cout << "\nConfirm " << operation << "? [y]es/[n]o ";
	getline(std::cin, confirm);

	if(confirm == "y" || confirm == "Y")
		return true;
	else if(confirm == "n" || confirm == "N")
		return false;
	else
		throw std::domain_error("Invalid option");
}

void replace_newline(string& to_replace)
{
	auto pos = to_replace.find("\\n");
	while(pos != string::npos)
	{
		to_replace.replace(pos, 2, "\n");
		pos = to_replace.find("\\n", pos);
	}
}

int process_input(int argc, char** argv, const database& db)
{
	// Use default system locale
	{
		boost::locale::generator gen;
		std::locale::global(gen(""));
	}

	CLI::App app("Rate and search politicians");
	app.require_subcommand(1);

	auto reg = app.add_subcommand("register",
			"Register a new politician in the database");
	string name, party("None"), info("N/A");
	reg->add_option("-n,--name", name, "Name of the politician")->required();
	reg->add_option("-p,--party", party, "Party of the politician");
	reg->add_option("-i,--info", info, "General information for the politician");
	reg->callback([&name, &party, &info, &db]
	{
		replace_newline(info);
		// Converts name and party to uppercase to make these columns case insensitive
		// on every SQL query/operation
		politician p(to_upper(name), to_upper(party), move(info));
		p.print_data();
		if(confirm_operation("insertion"))
		{
			if(db.insert_to_politician(p))
				std::cout << "Successfully inserted.\n";
			else
				std::cerr << "Insertion failed.\n";
		}
	});

	auto rate = app.add_subcommand("rate", "Add a new rate to an existing politician");
	short points;
	string desc("N/A");
	rate->add_option("-n,--name", name, "Name of the politician")->required();
	rate->add_option("-p,--party", party, "Party of the politician");
	rate->add_option("-r,--rate", points,
			"Rating points to add/subtract [-5 to 5]")->required();
	rate->add_option("-d,--description", desc, "Description or reason for the rate");
	rate->callback([&name, &party, &desc, &points, &db]
	{
		replace_newline(desc);
		// Converts name and party to uppercase to make these columns case insensitive
		// on every SQL query/operation
		rating r(to_upper(name), to_upper(party), move(desc), points);
		r.print_data();
		if(confirm_operation("rating"))
		{
			if(db.insert_to_ratings(r))
				std::cout << "Successfully inserted.\n";
			else
				std::cerr << "Insertion failed.\n";
		}
	});

	auto update = app.add_subcommand("update", "Update the party of a politician");
	string new_party("None");
	update->add_option("-n,--name", name, "Name of the politician")->required();
	update->add_option("-o,--old-party", party, "Old party");
	update->add_option("-p,--new-party", new_party, "New party");
	update->callback([&name, &party, &new_party, &db]
	{
		// Converts name, party and new_party to uppercase to make these columns case
		// insensitive on every SQL query/operation
		politician_update p(to_upper(name), to_upper(party), to_upper(new_party));
		p.print_data();
		if(confirm_operation("update"))
		{
			if(db.update_party(p))
				std::cout << "Successfully updated.\n";
			else
				std::cerr << "Update failed. The politician was not found.\n";
		}
	});

	auto del = app.add_subcommand("delete", "Delete a politician of the database");
	del->add_option("-n,--name", name, "Name of the politician")->required();
	del->add_option("-p,--party", party, "Party of the politician");
	del->callback([&name, &party, &db]
	{
		// Converts name and party to uppercase to make these columns case insensitive
		// on every SQL query/operation
		politician_core p(to_upper(name), to_upper(party));
		p.print_data();
		if(confirm_operation("deletion"))
		{
			if(db.delete_politician(p))
				std::cout << "Successfully deleted.\n";
			else
				std::cerr << "Deletion failed. The politician was not found.\n";
		}
	});

	auto search = app.add_subcommand("search", "Search options");
	search->require_subcommand(1);

	auto search_name = search->add_subcommand("name", "Search a politician by name");
	search_name->add_option("name", name, "Name of the politician")->required();
	search_name->callback([&name, &db]
	{
		vector<politician> politicians = db.get_politician_by_name(to_upper(name));
		for_each(politicians.begin(), politicians.end(), [](const politician& p)
		{
			p.print_data();
			std::cout << "\n";
		});
		std::cout << politicians.size() << " results returned.\n";
	});

	auto search_party = search->add_subcommand("party",
			"Show all politicians belonging to a party");
	search_party->add_option("party", party, "Party to be searched")->required();
	search_party->callback([&party, &db]
	{
		vector<politician> politicians = db.get_politicians_by_party(to_upper(party));
		for_each(politicians.begin(), politicians.end(), [](const politician& p)
		{
			p.print_data();
			std::cout << "\n";
		});
		std::cout << politicians.size() << " results returned.\n";
	});

	auto search_ratings = search->add_subcommand("ratings",
			"Show all ratings belonging to a politician");
	search_ratings->add_option("-n,--name", name, "Name of the politician")->required();
	search_ratings->add_option("-p,--party", party, "Party of the politician");
	search_ratings->callback([&name, &party, &db]
	{
		vector<rating> ratings = db.get_politician_ratings(
				politician_core(to_upper(name), to_upper(party)));
		for_each(ratings.begin(), ratings.end(), [](const rating& r)
		{
			r.print_data();
			std::cout << "\n";
		});
		std::cout << ratings.size() << " results returned.\n";
	});

	auto search_all = search->add_subcommand("all",
			"Show all politicians ordered by highest rating");
	bool _reverse(false), full(false);
	search_all->add_flag("-r,--reverse", _reverse, "Order by lowest to highest rating");
	search_all->add_flag("-f,--full", full,
			"Includes the description and rating points of each politician");
	search_all->callback([&_reverse, &full, &db]
	{
		string search_order = _reverse ? "ASC" : "DESC";
		if(full)
		{
			vector<politician> politicians = db.get_all_politicians(search_order);
			for_each(politicians.begin(), politicians.end(), [](const politician& p)
			{
				p.print_data();
				std::cout << "\n";
			});
			std::cout << politicians.size() << " results returned.\n";
		}
		else
		{
			vector<politician_core> politicians = db.get_politicians_compact(search_order);
			for_each(politicians.begin(), politicians.end(), [](const politician_core& p)
			{
				p.print_data();
				std::cout << "\n";
			});
			std::cout << politicians.size() << " results returned.\n";
		}
	});

	CLI11_PARSE(app, argc, argv);

	return 0;
}
