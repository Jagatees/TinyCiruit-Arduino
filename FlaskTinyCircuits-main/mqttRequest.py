import paho.mqtt.client as mqtt
import time
import apiRequests
import threadconfig
import googlecal


class mqttRequest:

    def __init__(self):
        self.MQTT_BROKER = "127.0.0.1"
        # self.MQTT_BROKER  = "172.20.10.2"
        #self.MQTT_BROKER = "192.168.76.192"
        self.MQTT_PORT = 1883
        self.MQTT_TOPIC = ['Weather/Request', 'Weather1/Request','GPT/Request','Googlecal/Request','Studenthelp','Attendance' ]
        self.MQTT_TOPICRESPONSE = ['Weather/Response','GPT/Response','Googlecal/Response']

        self.MQTT_KAHOOT = ['HootHoot/Request']
        self.MQTT_KAHOOTRESPONSE = ['HootHoot/Response']

        self.queue = [] #contain the request (it should be the topic name)

        # The callback for when the client receives a CONNACK response from the server.
        self.received_message = None
        self.receivedmessagetopic = None

        # this is to clear received message and topic when the reloading functions
        self.currentdealingtopic = None
        self.currentdealingtopicresponse = None

        self.client = mqtt.Client()
        self.client.on_message = self.on_message
        self.option1count = 0
        self.option2count = 0
        self.studentinneed = []
        self.attendance = []


        #self.x = apiRequests.apiRequests()

    def on_message(self,client, userdata, message):
        print('receiving something')
        self.received_message = message.payload.decode()
        self.receivedmessagetopic = message.topic

#!================================================================
#! initial stage for flask
#!================================================================
    def startflask(self):
        #check if it is connect to anything if so disconnect 
        if self.client.is_connected():
            self.client.disconnect()
        #publish false to hoothoot/start, Googlecal, Weather, GPT, Studenthelp, Attendance
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
        self.client.publish('HootHoot/Start', 'False')
        self.client.publish('Googlecal/Request', 'False')
        self.client.publish('Weather/Request', 'False')
        self.client.publish('GPT/Request', 'False')
        self.client.publish('Studenthelp', 'False')
        self.client.publish('Attendance', 'False')

        self.client.disconnect()

#!================================================================
#! kahoot purposes
    def startKahoot(self):
        #PUBLISH a start message to the mqtt server 
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
        self.client.publish('HootHoot/Start', 'True')
        self.client.disconnect()



    def sendOptions(self,option = []):
        #publish the options to mqtt
        options = ''
        # for option in option:
        #     options += option + ','
        #options = option['option1'] + ',' + option['option2']
        option1 = option['option1']
        option2 = option['option2']
        answer = option['correctAnswer'] 
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
        #self.client.publish('Hoothoot/Response', options)
        self.client.publish('HootHoot/Question1/Option1', option1)
        self.client.publish('HootHoot/Question1/Option2', option2)
        self.client.publish('HootHoot/Question1/ProfAnswer', answer)
        self.client.disconnect()

    def listenOptions(self,option = []):

        print(f'listening for {option}')

        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)

        self.client.subscribe('HootHoot/Answer')
        self.client.loop_start()

        while self.received_message is None:
            # use the varialbe in the file running this function to stop the loop 
            if threadconfig.stopworkhoothoot == True:
                print('stopworkhoothoot')
                self.client.loop_stop()  # Stop the loop
                self.client.disconnect()
                #reset everything 
                self.received_message = None
                self.receivedmessagetopic = None
                print('disconnected from mqtt kahoot')
                return
            
            # stop thread if the page is closed 

            pass
        #print out the message when received and end the function 
        print('ans received')
        print(self.received_message)
        option1 = option['option1']
        option2 = option['option2']
        
        if self.received_message == option1:
            print('option1 selected')
            self.option1count += 1
            print(self.option1count)
        elif self.received_message == option2:
            print('option2 selected')
            self.option2count += 1
            print(self.option2count)

        self.client.loop_stop()  # Stop the loop
        self.client.disconnect()
        #rerun the function 
        self.received_message = None
        self.receivedmessagetopic = None

        self.listenOptions(option)
#!==================================================================


#!!! port listener
#!==================================================================

    def subscribeListening(self):
        print('listening in portlistener')
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)

        for topic in self.MQTT_TOPIC:
            self.client.subscribe(topic)
            self.client.loop_start()

        print(f'subscribed to topics {self.MQTT_TOPIC}')
        count = 0
        while self.received_message is None:
            count +=1
            time.sleep(1)
            if count == 10:
                print ('no message received in past 10 seconds')
                #reset connection
                for topic in self.MQTT_TOPIC:
                    self.client.unsubscribe(topic)
                    self.client.loop_stop()
                self.client.disconnect()
                self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
                for topic in self.MQTT_TOPIC:
                    self.client.subscribe(topic)
                    self.client.loop_start()
                count = 0 

            if threadconfig.stopworkportlistener == True:
                for topic in self.MQTT_TOPIC:
                    self.client.unsubscribe(topic)
                    self.client.loop_stop()
                #! self.client.loop_stop()  # Stop the loop
                self.client.disconnect()
                #rerun the function 
                self.received_message = None
                self.receivedmessagetopic = None
                print('stopworkportlistener')
                return
            pass

        #print out the message when received and end the function 
        print('things received')
        print(self.received_message)
        print(self.receivedmessagetopic)
        #!!!!!!!!!!!!!!!!!!!if condition, if string = True, add the topic to the queue!!!!!!
        if self.receivedmessagetopic == 'Weather/Request' and self.received_message == 'True':
            self.queue.append('Weather/Request')
            self.currentdealingtopic = 'Weather/Request'
            self.currentdealingtopicresponse = 'True'

        elif self.receivedmessagetopic == 'GPT/Request':

            self.queue.append(f'GPT/Request/{self.received_message}')
            self.currentdealingtopic = 'GPT/Request'
            self.currentdealingtopicresponse = self.received_message
        
        elif self.receivedmessagetopic == 'Googlecal/Request' and self.received_message == 'True':
            self.queue.append('Googlecal/Request')
            print('googlecal appended')
            self.currentdealingtopic = 'Googlecal/Request'
            self.currentdealingtopicresponse = 'True'
        
        elif self.receivedmessagetopic == 'Studenthelp':
            self.studentinneed.append(self.received_message)
            print('studenthelp appended')
        
        elif self.receivedmessagetopic == 'Attendance':
            self.attendance.append(self.received_message)
            print('attendance appended')

        


        #reset connection after receiving message
        self.received_message = None
        self.receivedmessagetopic = None
        for topic in self.MQTT_TOPIC:
            self.client.unsubscribe(topic)
            self.client.loop_stop()
        self.client.disconnect()
        #rerun the function 


        self.subscribeListening()

    def publishResponse(self):
        #every 1second check the queue, if theres something go do the topic
        while True:
            

            if threadconfig.stopworkportlistener == True:
                #self.client.disconnect()
                print('stopworkportlistener')
                return
            
            if self.queue != []:
                print('something in queue')
                self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
                print (f'request from queue: {self.queue[0]}')
                
                if self.queue[0] == 'Weather/Request':
                    print('Weather/Request')
                    #!!!weather function!
                    #!!! ===============
                    #weather =self.x.get_weather()
                    weather = apiRequests.get_weather()
                    print(weather)
                    #!!!===============
                    self.client.publish('Weather/Response', 'weather')
                    self.client.publish('Weather/Response', weather)
                    self.queue.pop(0)
                    print('Weather/Response sent')
                    time.sleep(1)

                elif self.queue[0].split('/')[0] == 'GPT':
                    print('GPT/Request')
                    #!!!gpt function!
                    #!!! ===============
                    question = self.queue[0].split('/')[2]
                    print(f'question is {question}')
                    response = '' 

                    #responses = self.x.askgpt(question)
                    responses = apiRequests.askgpt(question)
                    for i in range(0,len(responses)):
                        # combine them into 1 string split by commas 
                        response += responses[i] + ','
                    print(response)
                    #!!!===============
                
                    self.client.publish('GPT/Response', response)
                    self.queue.pop(0)
                    print('GPT/Response sent')
                    time.sleep(1)
                
                elif self.queue[0] == 'Googlecal/Request':
                    print('Googlecal/Request')
                    #!!!googlecal function!
                    #!!! ===============

                    events = googlecal.main()
                    #breakdown events into 1 string
                    eventstring = ''

                    for event in events:
                        eventstring = event + ','
                    if eventstring == '':
                        eventstring = 'No events today'
                    #!!!===============
                    self.client.publish('Googlecal/Response', eventstring)
                    self.queue.pop(0)
                    print('Googlecal/Response sent')
                    time.sleep(1)
                
                # #for the studenthelper
                # elif self.queue[0].split('/')[0] == 'Studenthelp':
                #     #!!!studenthelp function!
                #     #!!! ===============
                #     studentname = self.queue[0].split('/')[1]
                #     print(f'studentname is {studentname}')
                #     self.studentinneed.append(studentname)
                #     #!!!===============
                #     self.queue.pop(0)
                #     time.sleep(1)


    # def studenthelper(self):
    #     #create a mqtt client with the settings to listen so it wont clash with the other mqtt client 
    #     studenthelperclient = mqtt.Client()
    #     studenthelperclient.on_message = self.on_message
    #     studenthelperclient.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
    #     studenthelperclient.subscribe('Studenthelp')
    #     studenthelperclient.loop_start()
    #     receivedstudentmessage = None 
    #     print('listening for studenthelper')
    #     while receivedstudentmessage is None:
    #         if threadconfig.stopworkportlistener == True:
    #             studenthelperclient.loop_stop()
    #             studenthelperclient.disconnect()
    #             print('stopStudenthelper')
    #         pass
    #     studenthelperclient.loop_stop()
    #     studenthelperclient.disconnect()
    #     self.studentinneed.append(receivedstudentmessage)
    #     self.studenthelper()
#!----------------------------------------------------------------------------------
#!!! the testing functions
#!==============================================    ====================
    def listentomqtt(self):
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
        self.client.subscribe('Test/Request')
        self.client.loop_start()
        while self.received_message is None:
            pass
        self.client.unsubscribe('Test/Request')
        self.client.loop_stop()
        self.client.disconnect()
        return self.received_message
    
    def publishmqtt(self,message):
        self.client.connect(self.MQTT_BROKER, self.MQTT_PORT, 60)
        self.client.publish('Test/Response', message)
        self.client.disconnect()
        return 'sent'

