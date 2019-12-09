## C++ Implementation of Full-Service Welcome Mat Logic Circuit

##### Made by: Nikita Voronin, Eric Argenio, Trevor Ward

### Project Logic
The idea behind our project is that it can automatically control lights in the room depending on the pressure sensors on the beds. Logic is simple: 

  •	If there are no people on the beds -> turn light on.

  •	If there is at least one person sleeping -> turn light off.

  •	If everybody is sleeping and somebody steps on mat -> turn light on (intruder alert).
  
### Work Breakdown
We have split the project in three parts which helped us to manage time efficiently and simplify each next step:

  1.	Creating flow chart, logic circuit, and Boolean expressions. (Trevor Ward)

  2.	Creating a programming checklist based on the flow chart and logic circuit. (Eric Argenio)

  3.	Implementing an application in C++.	(Nikita Voronin)
 
### Circuit Diagram

As can be seen on the diagram below, our logic circuit consists of 5 inputs, 2 OR gates, 2 AND gates, 1 NOR gate, and a D Flip Flop with a clock connected to it. The output of this circuit represents if the light should be on or off. Gate I4 is the most interesting to us since it controls the intruder alert case, which occurs when all 4 bed inputs are 1 and the light is on.

![cIRC](https://user-images.githubusercontent.com/23469990/70302125-3fd55680-17ca-11ea-83f3-cd436d055d5a.png)

### Event Flow Chart

![flow_chart](https://user-images.githubusercontent.com/23469990/70399984-1ef14900-19f6-11ea-9c9a-55ea6b704727.png)

### Programming Checklist
Programming language: C++17 (standard library only)

#### D Flip Flop class:

  • Boolean variables: state, input, and clock
  • Methods to set/get input and clock
  • Method to set state equal to input if the clock just went from 0 to 1

#### Global Functions: 

  • Functions that represent: 2 OR gates, 2 AND gates, and a NOR gate
  • Function that accepts initial inputs as parameters (constant array of bed inputs reference, light state reference, constant clock state reference, and a D Flip Flop reference) and runs logic circuit using the methods specified above.
  • Main function to setup inputs and call the function specified above.

#### Used Variables:
  
  • const std::array<bool, SIZE>& -> (represents beds inputs)
  • boolean variables representing light and clock state
  • D Flip Flop instance
