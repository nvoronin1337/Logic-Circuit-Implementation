#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <ctime>

const int NUM_OF_BEDS = 4;
#define FILE "logs.txt"

#pragma warning(disable : 4996)

inline const char* const BoolToString(bool b)
{
	return b ? "1" : "0";
}

class Dorm_Room {
private:
	// Light and Beds
	int people_in_room = 0;
	bool light_status = false;
	std::array<bool, NUM_OF_BEDS> beds;
public:
	Dorm_Room() {
		this->beds = { 0,0,0,0 };
	}

	// Getters 
	bool get_light() {
		return this->light_status;
	}

	std::array<bool, NUM_OF_BEDS>& get_beds() {
		return this->beds;
	}

	// Light manipulations
	void light_on() {
		this->light_status = true;
	}

	void light_off() {
		this->light_status = false;
	}

	
	// Bed manipulations
	void lay_in_bed(int bed) {
		this->beds.at(bed) = true;
	}

	void leave_bed(int bed) {
		this->beds.at(bed) = false;
	}

	void leave_all_beds() {
		for (bool& bed : this->beds) {
			bed = false;
		}
	}
};


class Simulation {
private:
	Dorm_Room room;
public:
	Simulation(Dorm_Room& room) {
		this->room = room;
	}

	int step_on_mat() {
		if (std::all_of(std::begin(this->room.get_beds()),
			std::end(this->room.get_beds()),
			[](bool bed)
			{
				return !bed; 
			})) {
			std::cout << "All beds are empty!\n";
			this->room.light_on();
			return 0;
		}
		else if (std::all_of(std::begin(this->room.get_beds()),
			std::end(this->room.get_beds()),
			[](bool bed)
			{
				return bed; // or return !i ;
			})) {
			std::cout << "All beds are full!\n";
			this->room.light_on();
			std::cout << "INTRUDER ALERT!!!\n";
			return 2;
		}
		else {
			std::cout << "Somebody is sleeping...\n";
			this->room.light_off();
			return 1;
		}
	}

	void lay_on_bed(int bed) {
		if (bed >= 0 && bed < NUM_OF_BEDS) {
			this->room.lay_in_bed(bed);
			std::cout << "Somebody is now sleeping at bed " << bed << ".\n";
			this->room.light_off();
		}
		else {
			std::cout << "There are only " << NUM_OF_BEDS << " beds!";
		}
	}

	std::string status_to_str() {
		std::string status = "";
		status.append("Beds status: ");
		for (bool bed : this->room.get_beds()) {
			status.append(BoolToString(bed));
		}
		status.append("\nLight status: ");
		if (this->room.get_light()) {
			status.append("1");
		}
		else {
			status.append("0");
		}
		status.append("\n");
		return status;
	}

	void status_to_file(std::string extra = "") {
		std::ofstream file_out;
		file_out.open(FILE, std::ios::out|std::ios::app);
		if (extra != std::string("")) {
			file_out << extra << '\n';
		}
		file_out << this->status_to_str();
		// current date/time based on current system
		time_t now = time(0);
		// convert now to string form
		char* dt = ctime(&now);
		
		file_out << "Date: " << dt << '\n';
		

		file_out.close();
	}
	Dorm_Room& get_room() {
		return this->room;
	}
};


int main()
{
	Dorm_Room room_1200 = Dorm_Room();
	Simulation simulation = Simulation(room_1200);

	bool running = true;
	while (running) {
		int choice = -1;
		std::cout << "1. Check bed and light status.\n";
		std::cout << "2. Enter room (step on mat).\n";
		std::cout << "3. Lay in bed.\n";
		std::cout << "4. Leave bed.\n";
		std::cout << "5. Cancel intruder alert.\n";
		std::cout << "6. Exit.\n";
		std::cout << "Choose an action: ";
		std::cin >> choice;
		std::cout << std::endl;
		switch (choice) {
		case 1:
			std::cout << "************\n";
			std::cout << simulation.status_to_str();
			std::cout << "************\n";
			break;
		case 2:
		{
			int response = simulation.step_on_mat();
			switch (response) {
			case 0: {
				simulation.status_to_file("Person enters room. All beds empty.");
				break;
			}
			case 1:
			{
				simulation.status_to_file("Person enters room. Somebody is sleeping.");
				break;
			}
			case 2:
			{
				simulation.status_to_file("Person enters room. ALERT!!!");
				break;
			}
			};

			break;
		}
		case 3:
		{
			int bed = -1;
			std::cout << "There are " << NUM_OF_BEDS << " total beds in this room.\n";
			std::cout << "Please choose a bed to lay in: ";
			std::cin >> bed;
			simulation.lay_on_bed(bed);
			simulation.status_to_file("Person goes to sleep");
			break;
		}
		case 4: {
			if (std::all_of(std::begin(simulation.get_room().get_beds()),
				std::end(simulation.get_room().get_beds()),
				[](bool bed)
				{
					return !bed;
				})) {
				std::cout << "All beds are empty!\n";
			}
			else {
				int bed = -1;
				std::cout << "Please choose a bed to empty: ";
				for (int bed = 0; bed < NUM_OF_BEDS; bed++) {
					if (simulation.get_room().get_beds().at(bed) == true) {
						std::cout << bed << ' ';
					}
				}
				std::cout << std::endl;
				std::cin >> bed;
				simulation.get_room().leave_bed(bed);
				if (std::all_of(std::begin(simulation.get_room().get_beds()),
					std::end(simulation.get_room().get_beds()),
					[](bool bed)
					{
						return !bed;
					})) {
					std::cout << "Now all beds are empty! Turning light on...\n";
					simulation.get_room().light_on();
				}
			}
			simulation.status_to_file("Person got up from bed");
			break;
		}
		case 5:
			if (simulation.get_room().get_light()) {
				simulation.get_room().light_off();
				std::cout << "Intruder alert was manually canceled!\n";
				simulation.status_to_file("Intruder alert was manually canceled!");
			}
			else {
				std::cout << "There is no active alert!\n";
			}
			break;
		case 6:
			running = false;
		default:
			std::cout << "Please choose a valid menu item.\n";
			break;
		}
		std::cout << std::endl;
	}
}