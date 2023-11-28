from flask import Flask, render_template, jsonify,request
import paho.mqtt.client as mqtt
import threading 
import time
import apiRequests
#import telebotcircuit

app = Flask(__name__, template_folder='template')

#mqtt settings 
MQTT_BROKER = "127.0.0.1"

MQTT_BROKER  = "172.20.10.2"
MQTT_PORT = 1883
MQTT_TOPIC = ['Weather/Request', 'Weather1/Request','GPT/Request','Hoothoot/Request']
MQTT_TOPICRESPONSE = ['Weather/Response','GPT/Response','Hoothoot/Response']

queue = [] #contain the request (it should be the topic name)

# The callback for when the client receives a CONNACK response from the server.
received_message = None
receivedmessagetopic = None

def on_message(client, userdata, message):
    global received_message
    global receivedmessagetopic
    print('receiving something')
    received_message = message.payload.decode()
    receivedmessagetopic = message.topic


client = mqtt.Client()
client.on_message = on_message


#!================================================================
#! kahoot purposes

option1count = 0
option2count = 0
def sendOptions(option = []):
    #publish the options to mqtt
    options = ''
    for option in option:
        options += option + ',' 
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.publish('Hoothoot/Response', options)
    client.disconnect()

def listenOptions(option = []):
    global option1count
    global option2count
    global received_message
    global receivedmessagetopic
    
    print(f'listening for {option}')

    client.connect(MQTT_BROKER, MQTT_PORT, 60)

    client.subscribe('Hoothoot/Request')
    client.loop_start()

    while received_message is None:
        pass
    #print out the message when received and end the function 
    print('ans received')
    print(received_message)
    option1 = option['option1']
    option2 = option['option2']
    
    if received_message == option1:
        print('option1 selected')
        option1count += 1
        print(option1count)
    else:
        print('option2 selected')
        option2count += 1
        print(option2count)

    client.loop_stop()  # Stop the loop
    client.disconnect()
    #rerun the function 
    received_message = None
    receivedmessagetopic = None

    listenOptions(option)
#!==================================================================

def subscribeListening():
    global received_message
    global receivedmessagetopic

    print('listening')
    client.connect(MQTT_BROKER, MQTT_PORT, 60)

    for topic in MQTT_TOPIC:
        client.subscribe(topic)
    client.loop_start()

    while received_message is None:
        pass
    #print out the message when received and end the function 
    print('things received')
    print(received_message)
    print(receivedmessagetopic)
    #!!!!!!!!!!!!!!!!!!!if condition, if string = True, add the topic to the queue!!!!!!
    if receivedmessagetopic == 'Weather/Request' and received_message == 'True':
        queue.append('Weather/Request')
    elif receivedmessagetopic == 'GPT/Request':

        queue.append(f'GPT/Request/{received_message}')



    client.loop_stop()  # Stop the loop
    client.disconnect()
    #rerun the function 
    received_message = None
    receivedmessagetopic = None

    subscribeListening()

def publishResponse():
    #every 1second check the queue, if theres something go do the topic
    while True:
        if queue != []:
            if queue[0] == 'Weather/Request':
                print('Weather/Request')
                #!!!weather function!
                #!!! ===============

                #!!!===============
                client.publish('Weather/Response', 'The Weather Hot')
                queue.pop(0)
                print('Weather/Response sent')
                time.sleep(1)

            elif queue[0].split('/')[0] == 'GPT':
                print('GPT/Request')
                #!!!gpt function!
                #!!! ===============
                question = queue[0].split('/')[2]
                response = '' 
                responses = apiRequests.askgpt(question)
                for i in range(0,len(responses)):
                    # combine them into 1 string split by commas 
                    response += responses[i] + ','
                print(response)
                #!!!===============
             
                client.publish('GPT/Response', response)
                queue.pop(0)
                print('GPT/Response sent')
                time.sleep(1)



#----------------------------------------------------------------------------------


@app.route('/')
def hello():
    return render_template('index.html')


@app.route('/portlisten', methods=['GET', 'POST'])
def portlisten():
    global received_message
    global receivedmessagetopic
    # when get in,connect and subscribe to both topics 
    if request.method == 'GET':

        
        # Create and start a new thread for each iteration
        thread = threading.Thread(target=subscribeListening)
        thread.start()

        #create a thread to publish to the response topic
        thread2 = threading.Thread(target=publishResponse)
        thread2.start()


        #print out the message when received and end the functi
        return render_template('portlisten.html')



#send to mqtt
@app.route('/send', methods=['GET', 'POST'])
def mqttpublish():
    if request.method == 'POST':
        print(request.form['message'])
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.publish(MQTT_TOPIC, request.form['message'])

        client.disconnect()
        return render_template('index.html', message = 'sent')
    



#listen to mqtt 
@app.route('/listen', methods=['GET', 'POST'])
def mqttlisten():
    global received_message
    if request.method == 'POST':
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        print(MQTT_TOPIC)
        client.subscribe(MQTT_TOPIC)
        client.loop_start()

        while received_message is None:
            pass  # Wait for a message to be received

        #if the message = "hello", then print out hello, else print out not friendly, unsubscribe and disconnect
        client.unsubscribe(MQTT_TOPIC)
        client.loop_stop()  # Stop the loop
        client.disconnect() 

        print(received_message)
        result = received_message
        received_message = None

        if result == "hello":
            print('hello')

            return render_template('index.html', message = 'hello')
        else:
            print('not hello')

            return render_template('index.html', message = result)




@app.route('/requestsHistory')
def requestHist():

    return render_template('requestsHist.html')

@app.route('/hoothoot', methods=['GET', 'POST'])
def hoothoot():
    if request.method == 'POST':
        print('post request received')
        print(request.get_json())
        #look for the special request tag in string
        if request.get_json() == 'special':
            print('special request received')
            print(option1count)
            print(option2count)
            return jsonify(option1 = option1count, option2 = option2count)
        
        #publish options to mqtt server then open up a port to keep listening for the response 
        #publish the options to mqtt
        option = request.get_json()
        print(type(option))
        print(option)
        print('options printed')
         #this is a list of options 
        sendOptions(option = option)
        #listen to mqtt (thread it )

        thread1 = threading.Thread(target=listenOptions, args=(option,))
        thread1.start()


        return render_template('hoothoot.html', message = 'sent')
    
    else:
        #PUBLISH a start message to the mqtt server 
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.publish('Hoothoot/Response', 'Start')
        client.disconnect()

    

    return render_template('hoothoot.html')



if __name__ == '__main__':
    
    app.run(debug=True)