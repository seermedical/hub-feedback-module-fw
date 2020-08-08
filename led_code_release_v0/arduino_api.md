# Arduino LED Control and Temperature/Humidity Sensor API

## Serial Communication

- Serial
- Baudrate: 9600

## Initialization Procedure

If the accelerometer fails to start, the arduino will send "Couldnt start\n" over serial

## Command Message Format

- 12 ascii (1 Byte) characters long
- The last character is the newline character **\n**
- first 10 characters are for LED control only
- if 10th character is the set to **'S'** then the first 9 characters are parsed for LED control
- the 11th character is used for requesting data (if it's set)

### LED Control 

#### Command Format (Hub -> Arduino)

Format: "*RRRGGGBBB*S0\n" where RRR GGG BBB are the integer value for *Red*, *Green*, and *Blue* respectively

Set the 10th character to **'S'**

### Data Request

#### Command Format (Hub -> Arduino)

Set the first 10 chars to **'0'**
Append the newline character ('\n') to the end of the command

- **'T'** *("0000000000T\n")* Internal Temperature value request
- **'H'** *("0000000000H\n")* Internal Humidity value request
- **'E'** *("0000000000E\n")* External Temperature value request
- **'I'** *("0000000000I\n")* External Humidity value request
- **'A'** *("0000000000A\n")* Max Acceleration value request and reset max acceleration counter

#### Response Format (Arduino -> Hub)

The string representation of the floating point number value of the data requested terminated by a newline character
