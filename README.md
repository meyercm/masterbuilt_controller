## Masterbuilt Smoker Controller

I purchased [this smoker](https://www.amazon.com/Masterbuilt-130B-Digital-Electric-Smoker/dp/B07CN38M23) in 2016, and now, in 2020, the small controller unit that chooses cook temp and duration has died.
This library currently provides simple setpoint control over the smoker using a PID control algorithm.
The board used was an Arduino nano, because it was the only 5V micro I had available on that critical day when the ribs were seasoned and the smoker wouldn't turn on.

### Future Work:

- [ ] Give it its own wifi (ESP32? but then need 5v logic and ADC)
- [ ] Simple webpage for control/status
- [ ] Add a duration to the parameters so that it shuts off automatically
- [ ] Upgrade the PID parameters to eliminate the need for a bias - I think it's overdamped


### Wiring:

I couldn't find any info online, so I did my own investigation:
 * Black - Ground
 * Blue  - +5V
 * Red   - Active-high (5V?) element control
 * White - Analog thermocouple line
 
### Thermocouple

Instead of repeating the formula, look [here](https://github.com/meyercm/masterbuilt_controller/blob/master/src/main.cpp#L43).

The derivation so far only has a handful of datapoints, and if I had to guess, would say that the effective cooking temp is actually below this reading (my calibration was done with a meat thermometer stuck in the smoke exhaust.


