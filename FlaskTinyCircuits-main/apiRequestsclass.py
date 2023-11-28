import openai
import requests



class apiRequests:
    
    def __init__(self):
        openai.api_key = "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"
        #self.openai.api_key = "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"
        self.api_key = '2a85ac8917029b99a7839adeeb23ab9a' #weather
        self.choices = []

    def askgpt(self,question): #!what kind of thai dish do you want?
        completion = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[
            #{"role": "system", "content": "The user is not able to handle spice and is vegetarian."},

            {"role": "user", "content": "Compose 4 unique replies of 16 characters or less to the message {}.".format(question)}
        ]
        )

        choices = str(completion.choices[0].message)[39:-3]
        print(choices)
        choice = choices.split("\\n")

        
        #remove the \\ in each choice as well as the number and the " "
        for i in range(0,len(choice)):
            choice[i] = choice[i].replace("\\","")
            choice[i] = choice[i][3:-1]   

        return choice


    # !!!! get weather 

    
    def get_weather(self):
        # Singapore latitude and longitude
        latitude = ("1.3667")
        longitude = ("103.8")
        # openweathermap endpoint
        url = (f"http://api.openweathermap.org/data/2.5/weather?lat={latitude}&lon={longitude}&appid={self.api_key}&units=metric")

        # Make a request to the openweathermap api
        response = requests.get(url)
        if response.status_code == 200:
            data = response.json()
            # weather condition ID
            weather_condition_id = data['weather'][0]['id']

            # Determine weather condition based on the condition ID
            if weather_condition_id == 800:
                weather_condition = ("Sunny")
            elif weather_condition_id // 100 == 8:
                weather_condition = ("Cloudy")
            elif weather_condition_id // 100 == 2 or weather_condition_id // 100 == 3 or weather_condition_id // 100 == 5:
                weather_condition = ("Rainy")
            else:
                weather_condition = ("Varies")

            # Get current, max, and min temp
            current_temperature = int(data['main']['temp'])
            max_temperature = int(data['main']['temp_max'])
            min_temperature = int(data['main']['temp_min'])

            # Return weather condition and temp
            return (f"Weather condition: {weather_condition}\n"
                    f"Temperature now: {current_temperature}°C\n"
                    f"Max temperature: {max_temperature}°C\n"
                    f"Min temperature: {min_temperature}°C")
        else:
            return "Failed to retrieve weather data"
        
