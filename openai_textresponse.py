import os
import openai
openai.api_key = #ask elise for key

completion = openai.ChatCompletion.create(
  model="gpt-3.5-turbo",
  messages=[
    #{"role": "system", "content": "The user is not able to handle spice and is vegetarian."},
    {"role": "user", "content": "Compose 4 replies of 16 characters or less to the message {}.".format("what kind of thai dish do you want?")}
  ]
)

choices = str(completion.choices[0].message)[39:-3]
print(choices)
choice = choices.split("\\n")
print(choice)
for i in range(0,len(choice)):
  print(choice[i]) #send the choices in the list to arduino thru mqtt
