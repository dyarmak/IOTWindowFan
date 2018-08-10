# IOTWindowFan

Background
I have an enclosed patio / deck in my condo. The wallshaker A/C unit vents directly into said patio. 
With the unit running and windows open it raises the temperature 10-15 degrees C. With the patio windows closed I've seen it bend the needle on my thermometer, we're talking 55+ degrees...
My poor plants! 

I've set out to design and build a custom-size, IOT-controllable, temperature regulating Window Fan!

My goal here was a fan that automatically turned on when I turned on the A/C unit. Knowing what I know about the patio temperature increase when the A/C is running, I knew I could use the temperature difference between outside and inside to tell when it was on! 
Plus, in the evenings, when the sun beams directly in the windows (west facing deck) it could turn itself on to cool it down.

Initially I wanted to buy a pre-made fan and just hack it with an arduino / ESP8266, adding temperature control settings and a web interface. 
However, I couldn't find a pre-made fan in store or online that would fit in my large sized deck windows (~50" opening). 
So, I got some plywood and started cutting! 
Okay, not quite...

I found a 12v UL listed laptop power supply in my junk drawer from an old work laptop that had been dropped (not by me). 
I decided to use some 120mm 12v computer fans because a) they were cheap, and b) I knew i could control them with an arduino and some transistors. 
They are probably not water proof, so rain infiltration could be an issue in the future. I've sat on my deck in quite a few rain storms with the windows open and not gotten wet, so my fingers are crossed!

The window height would allow for 8 or 9 fans stacked vertically.
I found and bought the fans from Newegg.ca, a pack of 4 CoolerMaster fans for $17, incl shipping? Naturally I bought 2 packs.

After a few proposed layouts, the misses made me go with a 1 W x 8 H stacked layout. 
I went with 2 arrays of 4 fans each, leaving a space in the middle for the electronics.
I would place temperature sensors inside and outside at the top of the box, (as well as one inside the electronics box) 


Each of the fans is rated at 44.7 CFM. 
In my calculations I assumed some losses due to their close proximity and treated them as 40 CFM.
8 fans x 40 CFM = 320 CFM from the fan array. I don't know much about HVAC but hopefully this air flow rate is in the ballpark.

My enclosed deck dimensions:  14' L x 7' W x 8' H = 785 cu. ft.
785 cu. ft. / 320 CFM = 2.45 
2 and a half minutes to exchange all the air in the room seems more than adequate.  


