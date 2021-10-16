## Eventsystem <span id="events"></span>

The [input_manager](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/event/input_manager.h) represents an abstraction layer to system events that are triggered by the hardware (keyboard, mouse, ...). This abstraction allows input events to be simulated using simple input data (JSON files). Making the eventsystem and other systems built on it such as a commmand-queue filled by hardware events very easy to test.

It allows various custom backends. The only limitation is that the backend defines a method poll_event().
This must return a `std::tuple<bool, event_t>` and can take a variable number of parameters. These parameters are passed from the `input_manager` to the backend.

```c++
struct possible_backend
{
   std::tuple<bool, event_t> poll_event();
};

```
The backend should also be able to convert the events provided by the eventsystem into the appropriate backend format. Two sample backends are already implemented: 
- [SDL2](https://www.libsdl.org/)
- [SFML](https://www.sfml-dev.org/)

### Dependencies
- [nlohmann/json](https://github.com/nlohmann/json)
