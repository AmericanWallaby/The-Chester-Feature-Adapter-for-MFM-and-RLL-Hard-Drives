"The Chester" Feature Adapter v1.0


DESCRIPTION:

An open-source solution designed for older systems originally containing ST-412/ST-506 MFM/RLL hard disk drives that have upgraded mass storage where the original disk drive may be retained for authentic spin-up and synchronized head seeking noise.

Chester is a small PCB designed to attach to the 34 pin access cable connector on the rear of any full height, half height, or 3.5" form factor MFM or RLL hard drive. If your system has been updated to a more modern CF or SD card or other solid state device for reliability but you miss the original seek noise, this simple to employ solution is for you. 


USABILITY AND OPERATION:


Chester asserts minimal control over the drive's head positioner servo, and modulates estimated proportional length seek requests with a degree of randomness based on input from the system's mass storage access LED. As a general rule of thumb, if your drive spins up and comes ready, it should be usable with Chester, even if the drive has become otherwise unreliable or unsuitable for data storage as these interface subsystems are not accessed.



USER OPTIONS:

JP1: [Passthru | DS] This jumper modifies the characteristic of the front panel LED output, when used. 

In Passthru mode, output from the updated mass storage controller LED is passed directly through to the front panel. This indicates actual mass storage activity not synchronized with MFM/RLL drive seeking.

In DS or Drive Select mode, the front panel LED will illuminate only when Chester is issuing seek commands and the drive select line is true. More authentic, but does not indicate actual mass storage activity. 

If your drive has a built-in LED it will operate in DS mode (same as it did originally) independent of and not affected by the setting of JP1.

The Python code provided for the Arduino is written for compatibility with most ST-412/506 consumer disk drives 10mb and greater. The code is limited to about 280 cylinders in the main loop which will prevent mechanical seek overrun on most drives. This number can be adjusted to affect the character of your drive. A lower number will result in a a more active drive with shorter, more frenetic seek bursts. A higher number will allow for more frequent and long seeks, which operate the head positioner at slightly reduced speed, similar to buffered seeks on very early drives. Adjust to your preference.

Note that some very early full height units may require modification of the seek timing spec. Some of these drives (and presumably 8" and larger form factor drives compatible with the ST-412 or 506 interface) with larger or heavier head actuator assemblies or sluggish steppers may require more time to settle between tracks. On these drives, a timing spec that is too fast will usually result in head actuator groaning with either no movement or erratic movement. For example, a 3MB full height Shugart 604 requires an additional millisecond to correctly position the servo on fast seeks. In most cases, and unless you're working with a much older or more obscure drive, modification of the code will probably not be necessary beyond the max cylinder step specification. This can be adjusted by changing delay(0) to delay(1) in the seek section of the code. 

The code will periodlcally recalibrate the drive to track 0 by issuing an intentional cylinder seek overrun, effectively returning the servo to it's mechanical stop. With some drives (Miniscribe deserves honorable mention here) this results in a loud buzzing or growl but will not physically damage the drive.


ASSEMBLY:

The Chester PCB is provided with the following inputs:

+5V: [+5V | GND] The PCB requires 5vdc input to power the Arduino. It's often possible to add a jumper from the + side of LED Input to the +5V connection as the LED is supplied directly by the system's 5v rail. While the Arduino is technically grounded through the J1/Winchester interface connection to the disk drive, a dedicated ground wire to the power supply or nearby ground point on the drive or system case is still recommended.

LED Input: [+5V | HD LED] Input from system's updated mass storage controller LED connection (required for LED sync operation, optional for software sync operation)

LED Output: [+5V | HD LED] Output to system front panel (optional)

J1 / Winchester: 34 Pin PCB mount style card edge socket

The Arduino can be soldered to either side of the board as long as pin 1 orientation is observed. The only specialty item required is a PCB-mount card edge socket.

Actual 34 pin PCB mount card edge sockets have become difficult to source. As of the date of this publication, there is a small supply of new old stock Texas Instruments p/n H421121-17 available through ebay.

A more economical option is a more commonly available 36 pin socket, p/n S-36M-2.54-5. These will solder directly to the Chester PCB after clipping the two extra pins 35 and 36. The extra mechanical width should not be an issue as long as care is taken to get the connector pin 1 properly aligned when installed. 

Right angle jumper pin headers with standard 2.54mm spacing are recommended for JP1 and optionally at J3 and J4 if LED pigtails are not to be hard wired.

The fully assembled Chester feature adapter attaches rigidly to the back of your MFM/RLL drive by the J1/Winchester connector. No additional mounting considerations should be necessary.

