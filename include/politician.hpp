#ifndef POLITICIAN_HPP
#define POLITICIAN_HPP

// Standard libraries
#include <iostream>

// External libraries
#include <input.hpp>

using std::string;

struct politician_core
{
	const string name;
	const string party;

	/** Constructor */
	politician_core(string name, string party);

	/** Prints all the data stored for politician_core */
	virtual void print_data() const;
};

struct politician_update : politician_core
{
	const string new_party;

	/** Constructor */
	politician_update(string name, string party, string new_party);

	/** Prints all the data stored for politician_update */
	void print_data() const;
};

struct politician : politician_core
{
	const string info;
	const short points;

	/** Constructor */
	politician(string name, string party, string info, const short points = 0);

	/** Prints all the data stored for politician */
	void print_data() const;
};

struct rating : politician_core
{
	const string description;
	const short points;
	const string date_time;

	/** Constructor */
	rating(string name, string party, string description, const short points,
			string date_time = "");

	/** Prints all the data stored for the rating */
	void print_data() const;
};

#endif
