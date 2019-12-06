// Computer Organization Comp-1200-01
// Nikita Voronin, Trevor Ward, Eric Argenio
// Final Project Logic Circuit implementation

/**
* This is a second version of the Welcome Mat Project.
* This version is designed to implement the logic circuit.
* It has implementations for logic gate and for the D Flip Flop.
* Usage: 
* Change beds and light statuses in main and see if the light will turn on indication an intruder alert or not.
*/
#include <iostream>
#include <array>
#include <algorithm>

// CLOCK
#define CLOCK_ON 1
#ifdef CLOCK_ON
#include <chrono>
#include <thread>
#define SLEEP_TIME 1000	// 1000 ms
#define CLOCK_F 0
#define CLOCK_T 1
#endif // !CLOCK_ON

#define NUM_OF_BEDS 4

typedef const std::array<bool, NUM_OF_BEDS>& beds_const_ref;

/**
* (I3 D Flip Flop)
* Represents I3 D Flip Flop from logic circuit
* State is a saved state of the flip flop (Q on I3 icon)
* Input is an input to the flip flop from D3 (D on I3 icon)
* Clock is the current state of the clock
*/
struct D_Flip_Flop {
	bool state = 0;
	bool input = 0;
	bool clock = 0;

	void set_clock(const bool& clock) {
		if (this->clock != clock) {
			this->clock = clock;
			if (this->clock == 1) {
				this->state = this->input;
			}
		}
	}

	void set_input(const bool& input) {
		this->input = input;
	}

	const bool& get_state() {
		return this->state;
	}
};


/**
* Returns std::string in format:
* 0000 (beds)
* 0 (light)
* 0 stands for False, 1 for True
* The boolean value represents if there is anyone laying in a bed or if the light is turned on.
*/
std::string inputs_to_str(beds_const_ref beds_inputs, const bool& light_status_input) {
	std::string inputs_str = "";
	for (const bool& bed_input : beds_inputs) {
		inputs_str.append(std::string(bed_input ? "1" : "0"));
	}
	inputs_str.append("\n" + std::string(light_status_input ? "1" : "0") + "\n");
	return inputs_str;
}


/** 
* (I1 NOR GATE)
* Represents I1 NOR gate from the circuit diagram
* Returns True only if all bed inputs are false.
*/
bool i1_nor_gate(beds_const_ref beds_inputs) {
	return std::all_of(beds_inputs.cbegin(), beds_inputs.cend(), [](bool input) { return !input; });
}


/**
* (I4 AND GATE) INTRUDER ALERT 
* Represents I4 AND gate from the circuit diagram.
* Returns True only if all bed inputs and light status are true.
*/
bool i4_and_gate(beds_const_ref beds_inputs, const bool& light_status) {
	return std::all_of(beds_inputs.cbegin(), beds_inputs.cend(), [](bool input) { return input; }) and light_status;
}


/**
* (D1 OR GATE)
* Represents D1 OR gate from the circuit diagram
* Returns True if light is True, or clock is True, or if both True
*/
bool d1_or_gate(const bool& light_status, const bool& clock) {
	return light_status or clock;
}


/**
* (D2 AND GATE)
* Represents D2 AND gate from the circuit diagram
* Returns True I1 and D1 gates output is True
*/
bool d2_and_gate(const bool& i1_output, const bool& d1_output) {
	return i1_output and d1_output;
}


/**
* (D3 OR GATE)
* Represents D3 OR gate from the circuit diagram
* Returns True I4 or D2 gates output is True, or if both outputs are True
*/
bool d3_or_gate(const bool& i4_output, const bool& d2_output) {
	return i4_output and d2_output;
}


void run_logic_circuit(beds_const_ref beds_inputs, bool& light_status_input, D_Flip_Flop d_flip_flop, const bool& clock_status) {
	std::cout << inputs_to_str(beds_inputs, light_status_input) << std::endl;

	// I Gates
	bool i1_output = i1_nor_gate(beds_inputs);
	bool i4_output = i4_and_gate(beds_inputs, light_status_input);

	// D Gates
	bool d1_output = d1_or_gate(light_status_input, d_flip_flop.get_state());
	bool d2_output = d2_and_gate(i1_output, d1_output);
	bool d3_output = d3_or_gate(i4_and_gate, d2_and_gate);

	// D Flip Flop save new input and update clock state
	d_flip_flop.set_input(d3_output);
	d_flip_flop.set_clock(clock_status);

	std::cout << "I1:" << i1_output << std::endl;
	std::cout << "I4:" << i4_output << std::endl;
	std::cout << "D1:" << d1_output << std::endl;
	std::cout << "D2:" << d2_output << std::endl;
	std::cout << "D3:" << d3_output << std::endl;
	std::cout << std::endl;

	std::cout << "D input: " << d_flip_flop.input << std::endl;
	std::cout << "D state: " << d_flip_flop.get_state() << std::endl;
	std::cout << std::endl;

	// If clock changed from 0 to 1 -> update light status
	if (clock_status) {
		light_status_input = d_flip_flop.get_state();
		std::cout << "Light status: " << light_status_input << std::endl;
	}
}


int main()
{
	beds_const_ref beds_statuses = { 1,1,1,1 };
	bool light_status = 0;
	bool clock_status = CLOCK_F;
	D_Flip_Flop d_flip_flop = D_Flip_Flop();

	while (1) {
		clock_status = !clock_status;
		run_logic_circuit(beds_statuses, light_status, d_flip_flop, clock_status);
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
		std::cin.get();
	}
	
	return EXIT_SUCCESS;
}