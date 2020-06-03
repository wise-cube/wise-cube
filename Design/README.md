# Design 

###### Authors: Giulia Del Citto, Diego Sonaglia, Roberto Sorce

## Introduction

In this document we're concerned about the design aspects of the IoT system proposed for the big project presented for the elective in Internet of Things at "La Sapienza University of Rome".

The aim of the project, developed in co-operation with the ***Museum of Classical Arts*** of Sapienza University, is to improve the user experience during the tour, offering information with innovative techniques and a different kind of entertainment.

## Product

###### Description

Wise-cube is an IoT device, provided by the museum: it is an interactive, multi-functional cube and an intelligent die that acts as a controller for an interactive quiz game.

The device has embedded:
   - ***ST-Microelectronics Nucleo board***
   - Dot matrix LED display
   - MPU 9250 sensor (Accelerometer + Gyroscope + Magnetometer)
   - NFC module to communicate with other sensors placed inside the many rooms of the museum. 

When the visitors begin the experience, it is given a **wise-cube**.

Each user, to start the interactive game, select his own __avatar__, by **shaking** the cube.
The **8x8 pixel avatar** identifies each player, and it is shown on the display.


![avatar](./avatar.png)

In the museum there will be various placements, the **cube activators**, that consist of **1 Screen** and **5 NFC Cards**.


###### User experience

Our product offers a funny and interactive user experience, suited either for a single user, or for multiple users together. Single visitors can even find other participants to play with, so to challenge them and even socialise.

Once arrived at the museum, the users are prompted to use the device to start this new tour experience and to play a game with the artworks, otherwise they can go on, taking the classical tour. 

If the users decides to take the alternative tour with our product, the museum staff will provide the few instructions to use it.

Wise-cube provides even an __interactive tour__, which the users can decide to choose, based on the age of the participants.
The device offers a dedicated tour experience for the kids, presenting a more simplified and enjoyable journey with respect to didactics, and their educational level and a regular tour for everyone.

When the users start their tour through the rooms of the museum, they will discover the ___activators___ where to place the IoT device near the artworks and it will display the corresponding QR-code to start the journey through the museum, guided by our prototype and playing the game level. Depending on the answer to the question, the player will discover a curiosity about the artworks.

Once the users have completed the visit at the museum, they have completed also all the game levels and at the end the user who have gained the highest score wins the game. 

## Personas

We identified the user target of our product, introducing the following personas:

![Persona1](./new_persona1.png )

1. Persona

   Name: Carlo  
   Surname: Verdone  
   Age: 8 y.o.   
   Gender: Male  
   Education: Elementary school   
   Career type: Student    
   Income: None  

__Persona 1__: Carlo is a 8 years old kid that lives in Rome, he attends the Elementary school. Carlo likes movies, to read novel books and playing board games and videogames on his smartphone.

![Persona2](./new_persona2.png)

2. Persona

   Name: Valentina   
   Surname: Rossi  
   Age: 46 y.o.   
   Gender: Female  
   Education: Doctoral degree  
   Career type: Professor  
   Income: 60.000€  

__Persona 2__: Valentina is a 46 years old mom, she's a Mechanical engineer Professor at La Sapienza University. She likes playing with her son Carlo and in the free time, she likes to visit museums.

__Scenario__: Saturday morning, Valentina and Carlo are free, they decide to spend the mornign together visiting a museum, so they decide to go the Museum of Classical Arts of Sapienza University, where Carlo can learn while having fun with his mom. 

![Persona3](./new_persona3.jpg)

3. Persona

   Name: Alessandra  
   Surname: Borghi  
   Age: 18 y.o.   
   Gender: Female  
   Education: High school  
   Career type: Student  
   Income: None

__Persona 3__: Alessandra attends the artistic high school in Tivoli. She likes to hang out with her friends, to practice sports, photography and painting. During the weekends she likes to visit and discover new places and especially to visit museums.

__Scenario__: The school organised a school trip in Rome, to visit the monuments and the museums of the capital city. Among the many museums of the city, the school decided to visit the Museum of Classical Arts of Sapienza university. Here they will be divided in groups and start visiting the museum with the new device. Thanks to the cube and the interactive games, they challenge each other groups discovering interesting facts about the artworks.

## Storyboard

![Storyboard](./story7.jpg)



![Link to the previous version of the file ](https://github.com/wise-cube/wise-cube/tree/1st-delivery/Design)
