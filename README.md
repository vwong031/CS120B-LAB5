# CS120B-LAB5

## Wiring Diagram
![image](https://user-images.githubusercontent.com/74090811/206875922-52b60225-1da4-4420-b4de-74a8b6d3d5f6.png)

## Exercise 1:
This exercise will use the joystick to control the direction and speed of the stepper motor. The 4 LED's will indicate what input the microcontroller recieved from the joystick.

* The stepper motor will initially rotate counterclockwise when the system turns on.
* Moving the joystick right will light up the right LED (pin 5) and transition the stepper motor to counterclockwise rotation.
* Moving the joystick left will light up the left LED (pin 2) and transition the stepper motor to clockwise rotation.
* Moving the joystick down will light up the bottom LED (pin 4) and reduce the rotational velocity of the stepper motor.
* Moving the joystick up will light up the top LED (pin 3) and increase the rotational velocity of the stepper motor. 
* Pressing the button on the joystick will turn on all of the LEDs at the same time and stop the motor from rotating. Releasing the button will turn the motor back on (it will start rotating again)
* After the joystick returns to the neutral position, you have two options:
* Turn off all of the LEDs; or
* Keep the left LED (pin 2) on whenever the stepper motor is rotating clockwise; keep the right LED (pin 5) on whenever the stepper motor is rotating counterclockwise;  
Demo Link: https://youtu.be/8WN_l4S2JQ0

## Exercise 2:
A common real-world use of stepper motors is to control heavy duty locks, for example, the type of lock that might be integrated into a safe. You will need to create a sequence of inputs that the lock will recieve from the joystick to open the "safe". You need to choose 4 individual inputs to be your passcode, an example passcode would be up-up-left-right.

* The sytem is initially "locked", all LED's should be off.
* The LED's should light up when the microcontroller recieves its input. For example if you move the joystick to the right the right LED will Pulse; this will confirm that the corresponding button has been pressed.
* When four inputs have been recieved one of two scenarios will take place, either the passcode was incorrect, or it was correct
  * If the passcode was incorrect, all four LED's will Pulse 3 times, then the system will wait for the next set of inputs.
  * If the passcode was correct all four LED's will stay on untill the safe is locked again
* When the system is locked and the passcode is entered correctly, the stepper motor shaft rotates 180 degrees counterclockwise to unlock the safe. 
* When the system is unlocked and the passcode is entered correctly, the stepper motor shaft rotates 180 degrees clockwise to lock the safe. 
  * When the system is unlocked and the passcode is entered incorrectly the nothing should happen.
* When the system is unlocked and the button on the joystick is press, the LED's will turn off. you can now set a new password.
  * The new password must obey the guidlines described above.
  * The LED's should light up when the microcontroller recieves its input. For example if you move the joystick to the right the right LED will Pulse; this will confirm    that the correct button has been pressed.
  * When four inputs have been read by the microcontroller the LED's will turn on again. You should be in the unlocked state again, but the new password needs to be input in order to lock the safe now.
* Inputting an incorrect passcode will not change the state of the system; the stepper motor shaft should not rotate. 

Demo Link: https://youtu.be/v20bnqR7ORw
