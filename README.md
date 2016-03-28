# diy-frsky-gps
Use any gps you like to interface with frsky telemetry. GPS Protocol and baudrate is detected automatically.

In config.h choose protocol:

For D-Series telemetry choose
```#define FRSKY_D```
For X-Series telemetry choose
```#define FRSKY_X```

You can change the update rate of the transmitted data with
```#define TELEMETRY_HZ 3```
This will change the update rate of GPS data. The higher the value the faster the transmission and the more accurate the position. D-Series a max of 3hz is recommended. On X-Series 5hz should work well.

Connect D9 to telemetry pin of your receiver.
