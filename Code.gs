const calendarids = ['primary']; // array of calendars ids, or the 'primary' keyword
const eventsToFilter = ['Work']; // strings of calendar event titles that you don't want included

function doGet(request) {

  const now = new Date();
  const fiveMinutesFromNow = new Date(now);
  fiveMinutesFromNow.setMinutes(fiveMinutesFromNow.getMinutes() + 5);

  // will be replaced with working hours api, when that's released
  if (now.getHours() < 8 || now.getHours() >= 16) {
    return ContentService.createTextOutput('n').setMimeType(ContentService.MimeType.TEXT);
  }

  let events = [];
  calendarids.forEach(id => events = events.concat(Calendar.Events.list(id, {"timeMin": now.toISOString(), "timeMax": fiveMinutesFromNow.toISOString(), "singleEvents": true}).items));

  events = events.filter(x => !eventsToFilter.includes(x.summary)).filter(x => x.transparency !== 'transparent');

  events.forEach(x => {
    x.start.jsdate = x.start.date ? new Date(x.start.date) : new Date(x.start.dateTime);
    x.end.jsdate = x.end.date ? new Date(x.end.date) : new Date(x.end.dateTime);
  });

  events = events.sort((a, b) => b.end.jsdate - a.end.jsdate); // ending last

  if (!events.length) {
    console.log('No events');
    return ContentService.createTextOutput('g').setMimeType(ContentService.MimeType.TEXT);
  } else if (events.some(x => x.start.jsdate <= now)) { // any have already started
    console.log('Event happening now');
    return ContentService.createTextOutput('r').setMimeType(ContentService.MimeType.TEXT);
  } else { // one or more events, but none have started yet
    console.log('Event starting soon');
    return ContentService.createTextOutput('y').setMimeType(ContentService.MimeType.TEXT);
  }
  
}
