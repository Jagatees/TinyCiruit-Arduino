from flask import Flask, render_template, jsonify,request,send_file
import threading 
from mqttRequest import mqttRequest
import threadconfig
import pandas as pd
import datetime

app = Flask(__name__, template_folder='template')
threadatwork = []

threadconfig.stopworkhoothoot = False
threadconfig.stopworkportlistener = False


@app.route('/' , methods=['GET', 'POST'])
def index():
    threadconfig.stopworkhoothoot = True
    threadconfig.stopworkportlistener = True
    x.startflask()
    return render_template('index.html')

@app.route('/hoothoot', methods=['GET', 'POST'])
def hoothoot():

    if request.method == 'POST':
        print('post request received')
        print(request.get_json())
        #look for the special request tag in string
        if request.get_json() == 'special':
            print('special request received')
            print(x.option1count)
            print(x.option2count)

            return jsonify(option1 = x.option1count, option2 = x.option2count)
        
        #publish opti   ons to mqtt server then open up a port to keep listening for the response 
        #publish the options to mqtt
        option = request.get_json()
        print(type(option))
        print(option)
        print('options printed')
         #this is a list of options 
        x.sendOptions(option = option)
        #listen to mqtt (thread it )

        thread1 = threading.Thread(target=x.listenOptions, args=(option,))
        thread1.start()
        threadatwork.append(thread1)


        return render_template('hoothoot.html', message = 'sent')
    
    else:
        threadconfig.stopworkhoothoot = False
        threadconfig.stopworkportlistener = True
        for thread in threadatwork:
            thread.join()
            threadatwork.remove(thread)
        #PUBLISH a start message to the mqtt server 
        x.startKahoot()
        x.option1count = 0
        x.option2count = 0
        print('publishing start message')
    

    return render_template('hoothoot.html')



@app.route('/portlisten', methods=['GET', 'POST'])
def portlisten():
    
    
    # when get in,connect and subscribe to both topics 
    if request.method == 'GET':
        threadconfig.stopworkportlistener = False #this is to stop the portlistener thread
        threadconfig.stopworkhoothoot = True #this is to stop the hoothoot thread 
        for thread in threadatwork:
            print('closing the thread')
            thread.join()

        x.startflask()
        # Create and start a new thread for each iteration
        thread = threading.Thread(target=x.subscribeListening)

        thread.start()

        #create a thread to publish to the response topic
        thread2 = threading.Thread(target=x.publishResponse)
        thread2.start()

        #create the studenthelper thread 
       # thread3 = threading.Thread(target=x.studenthelper)
      #  thread3.start()

        threadatwork.append(thread)
        threadatwork.append(thread2)
     #   threadatwork.append(thread3)

        # read pandas dataframe and return the it as a table , remove index column, header is the first row
        df = pd.read_csv('Studentshelper.csv',  header=0)
        print(df)
        table = df.to_html(classes='data',index=False) #classes is to style the table

        return render_template('portlisten.html',  table = table, titles = df.columns.values)
        #print out the message when received and end the functi
        return render_template('portlisten.html')
    else:
        #check the data in the get request if it is a studenthelp request
        if request.get_json() == 'studentsinneed':
            print('students in need request')
            studentsinneed = x.studentinneed
            x.studentinneed = [] #clear the list
            attendance = x.attendance
            x.attendance = [] #clear the list
            return jsonify(studentsinneed = studentsinneed, attendance = attendance)
        
        # #check the data in the get request if it is a attendance request
        # elif request.get_json() == 'attendance':
        #     print('attendance request')
        #     attendance = x.attendance
        #     x.attendance = [] #clear the list
        #     return jsonify(attendance = attendance)

        #check the data in the get request if it is a attendance request
        else:
            print('receiving attendance from studenthelper')
            print(request.get_json())
            attendancelist = request.get_json()
            #open the csv, find the name and update the attendance
            df = pd.read_csv('Studentshelper.csv',  header=0)
            print(df)
            #find the name and update the attendance
            for name in attendancelist:
                print(name)
                for index, row in df.iterrows():
                    if row['Students'] == name:
                        df.at[index, 'Attendance'] = 'Present'
                        break
            #i only want the first 2 columns
            df = df[['Students', 'Attendance']]
            #save the updated dataframe to csv, get todays date and save it as the filename
            now = datetime.datetime.now()
            filename = now.strftime("%Y-%m-%d") + '.csv'
            df.to_csv(f'Attendance/{filename}', index=False)

            #return jsonify(message = 'attendance received in flask')
            #return the file 
            return send_file(f'Attendance/{filename}', as_attachment=True)


    


#!==================================================================
#! testing mqtt with urls
#!==================================================================
#listen to mqtt 
@app.route('/listen', methods=['GET', 'POST'])
def mqttlisten():
    if request.method == 'POST':
        result = x.listentomqtt()

        if result == "hello":
            print('hello')

            return render_template('index.html', message = 'hello')
        else:
            print('not hello')

            return render_template('index.html', message = result)
        
#send to mqtt
@app.route('/send', methods=['GET', 'POST'])
def mqttpublish():
    if request.method == 'POST':
        print(request.form['message'])
        response = x.publishmqtt(request.form['message'])

        return render_template('index.html', message = response)
        
#!!!!!!!!!!!!!!==========================================================

x = mqttRequest()
if __name__ == '__main__':
    app.run('127.0.0.1', 5006, debug=True)