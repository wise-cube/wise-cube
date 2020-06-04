# Evaluation

###### Authors: Giulia Del Citto, Diego Sonaglia, Roberto Sorce

- [X] Brainstorming with design advisor
- [X] Mockup
- [X] Evaluation 1
- [ ] **Prototyping**
- [ ] Evaluation 2
- [ ] Product Development
- [ ] Evaluation 3 (feedback)

To properly evaluate our product, according to our case of study, we decided to resort to the _User Centered Design_ (__UCD__), considering different perspectives and factors that can affect the user experience.

We are going to use a mixture of both subjective and objective metrics to evaluate the quality of service (__QoS__) and to understand how to improve it.

### Evaluation goals

#### User Experience

  ###### Attractiveness 
 Overall impression of the product. We're concerned about the design and perception of the product from the user point of view. Does it catch the interest of users? Is it entertaining? 

  ###### Ease of interaction
 From the interaction point of view, we ask ourselves if the users find easy to get familiar with our product and to learn how to use it. Does the user feel in control of the interaction? Is it predictable? 
  
  ###### Motivation
 The solution we are proposing is motivating to justify the use of the product during the tour? Is it fun to use? Does the user perceive it as a useful tool to enrich the tour experience offered by the museum? Did it change your experience?


#### Technical Challenges
1. Detect pick-up motion 
2. Detect proper roll motion ( anti-cheating )
3. The cube will be probably be higly biased, due to its uneven mass distribution. 
   Anyway this can be easily fixed by getting statistics about the faces occurrency distribution and implement one of the following strategies:
   - In-game balancing: the faces that occurr less have the effect of advancing the player of  more tiles in the board.
   - Physical balancing: Adding weights on the faces that occurr less should make the outcome more uniform
4. Various sensors involved

   - PN532 NFC module

   - MPU-9250 Nine-Axis (Gyro + Accelerometer + Compass) Sensor

   - 8x8 LED Dot Matrix
5. Real Time communication between the cube and the webapp
6. Power consumption

### Evaluation 1 - Initial Feedback about the Idea

### Evaluation of the user experience

For what concerns the evaluation of each component of our system, we report below the metrics considered, always taking into account the __UCD__.

- __Subjective evaluation__: Our users have been subjected to fill a questionnaire, in order to gather suggestions to improve the experience. The data collected through a [Google form](https://forms.gle/j8imT1uCAk1TxU6y6), about the product and its usage, are useful to measure:
  - The user's learning rate with the help of our product
  - Impact on the museum tour using Wise-cube
  - Innovative interactive experience from the user's point of view
  - Familiar design

- __Objective evaluation__: Statistic analysis of data coming from different contexts (i.e. questionnaire); Due to COVID-19 emergency, some effective measurments are not possible to carry out on the field, at the moment. 
  - Statistic effective learning rate based on the game results 
  - Number of participants 
  - Number of game played 
  - Potentially growth of visitors of the museum, wrt the avarage of visitors without the proposed IoT system

### Evaluation 2 - User Experience and Technical Evaluation

#### Submit to the users the prototype, to retrieve important feedbacks about the game and the tour guide

In this second phase of evaluation, the users will already tried our product, hence they will be prompted again to fill a new form, with more questions regarding the ease of use of the overall product and the game.

- __Game Web app evaluation__: 
  - Ease of use of the gameplay
  - User interaction (attractive interface)
  - Severity of questions, regarding the level of difficulty (e.g. Kid's tour, standard tour)
  
### Evaluation of technical aspects

Technical aspect of the product evaluation includes:

- Failures/Anomalies detection during the communication among each component
- Responsiveness wrt the number of users currently using together the system
- Latency of messages exchanged by the modules

![Link to old Evaluation file](https://github.com/wise-cube/wise-cube/tree/1st-delivery/Evaluation)
