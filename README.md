# *Summarize the project and what problem it was solving.*
    
The gpiointerrupt project is a demonstration of a basic WiFi thermostat mockup. It solves the problem of automating the heating of a space to a set temperature, and logging information about that process to a cloud service.

# *What did you do particularly well?*

I feel like a made a very lightweight, easy to understand combination of task scheduler and procedures to accomplish the required functionality. I made sure not to overcomplicate the relevant procedures and program flow.

# *Where could you improve?*

The button handler procedure could be improved to provide better control. Currently, if the button is pressed at the same time as the triggering of the procedure, a double count occurs. With more effort, the repetition of the procedure could be tied to the button already being pressed down for one iteration.

# *What tools and/or resources are you adding to your support network?*

I previously had no experience with TI's microcontrollers and development toolchain (I had only used Atmel and ST microcontrollers). I think I will take a closer look at TI's catalog, as it was an enjoyable experience working with such a powerful chip.

# *What skills from this project will be particularly transferable to other projects and/or course work?*

I think the experience of debugging real hardware will be very valuable in my continuing CS degree. Its one thing to make an application and see it integrating with an OS, but the experience of seeing the direct physical result of code you wrote is on a whole other level.

# *How did you make this project maintainable, readable, and adaptable?*

This project will be easy to adapt to other TI microcontrollers due to extensive commenting, use of standard libraries, and minimal complexity in design.