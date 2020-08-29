# sec21

Linux [![Build Status](https://travis-ci.org/MichaelMiller-/sec21.svg?branch=master)](https://travis-ci.org/MichaelMiller-/sec21) 

Windows [![Build status](https://ci.appveyor.com/api/projects/status/4s6bg4yexj0cna45?svg=true)](https://ci.appveyor.com/project/MichaelMiller-/sec21)

Is a collection of useful functions and classes.
- [Units](#Units)
- [Eventsystem](#Eventsystem)


There also examples 
- [Viewer](#Viewer)

---------------------------------------

## Units
Modern compiletime [dimensional analysis](https://en.wikipedia.org/wiki/Dimensional_analysis) header only library based on [Boost](https://www.boost.org)-Units and the Boost-MPL example.


## Eventsystem
Das Eventsystem stellt eine Abstraktionsebene zu den direkten API-Aufrufen dar. Durch diese Absraktion lassen sich Eingabeevents mittels einfachen Inputddaten 
simulieren ("replayable"), dadurch wird as ganze System sehr einfach testbar.

Es läßt verschiedene "Benutzer-definierte" Backends zu. Die einzige Einschränkung dabei ist dass das Backend eine Methode poll_event() definiert.
Diese muss ein std::tuple<bool, event_t> zurückegeben und kann eine variable Anzahl von Parameteren annehmen. Die Parameter werden vom Eventsystem an 
das Backend durchgereicht.

```
struct possbile_backend
{
   std::tuple<bool, event_t> poll_event();
}

```

Das Backend sollte auch in der Lage sein die Events die das System zur Verfügung gestellt werden in das ensprechende Backend-Format umzuwandeln.

Wenn ddas Eventsystem getestet werden soll muss nur initial eine Liste von Events übergeben werden die das System abarbeitet.

---------------------------------------

## Viewer
The Viewer has it's own [description](viewer/README.md)

