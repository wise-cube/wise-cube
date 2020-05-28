### Comments that you received during the 1st delivery and how you have addressed them

We received very positive review of our project idea. The comments about the overall idea has being retained very good, even for the User Centered Design aspect, even though the way the user's opinions were collected was not well exposed.
Another key point to be improved was the evaluation document, which had to be more precise.

To solve the first point, we created a [Google form](https://forms.gle/j8imT1uCAk1TxU6y6) to obtain the user's opinions. 
The proposed form, aim to understand the habits of the users, to know how often the users visit a museum, if they have ever experienced an interactive tour and of course, to collect opinions about our new product.

Also, to improve the evaluation methodology, we corrected the evaluation document by adding more specific metrics we're interested in to measure.


### Changes made to your design, architecture and evaluation plan since the 1st delivery

##### Design
After we analysed the comments received during the first delivery presentation, and after we took into account the users advices, we decided to integrate a guide into our system. 
In this way the users can utilize our product to play a game and have, at the same time, an interactive guided tour, using the same device. 
In the museum the user will find two different type of activator: one for the game and one for the guide.

We also had to face another minor issue in the design of our IoT device. This made us reconsider the identity of our product: we decided that, in order to maximize the comfort during the journey, we had to rethink and reshape the device, not anymore into a cube, but in a simpler, smaller device, presenting a single embedded display.

Moreover, under a user suggestion, we have decided to create a specific tour experience, aimed to entertain kids and replace the cube with a smaller and more portable object that has just one display, to visualize the QR codes.

##### Architecture

###### Work in progress

##### Evluation plan 

The evaluation plan is the following:

- [X] Brainstorming with design advisor
- [X] Mockup
- [X] Evaluation 1
- [ ] **Prototyping**
- [ ] Evaluation 2
- [ ] Product Development
- [ ] Evaluation 3 (feedback)

At the moment, we evaluated only the general idea and the mockup, the next steps are the evaluation of the working prototype, with the help of our users, and the final product under a technical and user point of view.

### Brief presentation of the technical work done since the 1st delivery

Since the first delivery, we have developed the database and the relative queries to store the quiz, the guide information, and the data required to manage the system.
We have also developed a QR code generator to log in the guide and in the game.

- Databes Adapter
- Game Logic
- http Web Server (Flask) 


### Brief list of the functionality that is still missing and will be done till the 3rd and final delivery.

For what concern the code itself

- Riot-OS client
- MQTT/SN client interface
- MQTT server interface
- Riot-OS NFC reader 

For what concern infrastructure deployment

- Set up Google Cloud MQTT Broker
- Set up Google Cloud DB
- Set up Heroku App for HTTP/MQTT servers
- Set up MQTT/MQTT-SN adapter


### Brief presentation of the evaluation conducted since the 1st delivery.

The evaluation activity conducted since the first delivery, has been achieved through this [Google form](https://forms.gle/j8imT1uCAk1TxU6y6).
The form has been submitted from different type of customers, ranging over from 17 to 60 years old, with different education levels.
 
From the questionnaire, emerged that the larger part of the consumers visit the museums about 3-5 times in a year, and have tried at least once an interactive tour.

The results of the survey, show that in the user opinion, this kind of activity can be done by everyone, but with a differentiation of the tour, and more than 90% of the users think that this activity can support the visitors learning.

Furthermore, the large part of the users are willing to hold the cube during the tour, but using a smaller device they will appreciate more the experience.


### Brief list of what you expect to evaluate till the 3rd and final delivery
 
After completing the prototype, we plan to evaluate the game web app and the guide web app in terms of
usability, interaction and difficulty level using a new Google form.
We will evaluate also some technical aspects, such as the latency and the responsiveness of the system using statistic data.

The last evaluation will be conducted after the creation of the entire system, as the previous one, for the user experience and stastistics of the technical aspects. This is due to unforeseen circumstances beyond our control, preventing the collection of information directly on the field.

