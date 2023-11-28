import os
import datetime
from google_auth_oauthlib.flow import InstalledAppFlow
from google.oauth2.credentials import Credentials
from google.auth.transport.requests import Request
from googleapiclient.discovery import build

SCOPES = ['https://www.googleapis.com/auth/calendar.readonly']

all_events = []
eventstoday = []
event= ''

def main():
    creds = None
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file('token.json', SCOPES)

    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)

        with open('token.json', 'w') as token:
            token.write(creds.to_json())

    service = build('calendar', 'v3', credentials=creds)
    now = datetime.datetime.utcnow().isoformat() + 'Z'

    events_result = service.events().list(calendarId='primary', timeMin=now,
                                          singleEvents=True, orderBy='startTime').execute()
    events = events_result.get('items', [])

    if not events:
        print('No upcoming events found.')
    else:
        all_events.extend(events)
        print(all_events)
        # take out today's event
        for event in events:
            
            print(event)

            print('------------------')
        
        # create a list of events that are happening today,
        for event in events:
            try:
                dateforevent = event['start'].get('dateTime')[:10]
            except:
                dateforevent = event['start'].get('date')[:10]

            print(dateforevent)
            if dateforevent == datetime.datetime.now().strftime("%Y-%m-%d"):
                #add the event summary to the event
                event += f'Summary: {event["summary"]} , Start: {dateforevent}, End: {event["end"].get("dateTime")[:10]}'

                eventstoday.append(event)
        print(eventstoday)
        return eventstoday


if __name__ == '__main__':
    main()