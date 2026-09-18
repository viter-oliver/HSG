# HSG

HSG is a simple, lightweight, and easy to use graphics engine for creating HMI applications.
It is written in C++ and uses the OpenGL API for rendering.

Simply drag and drop your assets into the HSG IDE and automatically generate the referencable resouces for your application.
 ![image](vg/ide/gifs/image.gif)
 ![textblock](vg/ide/gifs/textblock.gif)
The coolest feature in HSG is the pipeline function. The pipeline is somewhat like a blueprint (in UE), where you simply drag some computing units and link them together to define an independent rendering pipeline.This is very useful for verifying and debugging certain shader code.
![pipeline](vg/ide/gifs/pipeline.gif)
![pipeline2](vg/ide/gifs/pipeline2.gif)
HSG uses a state machine to drive animations. Each state specifies a set of values for the properties being animated, such as position, rotation, scale, or color. During playback, the engine smoothly interpolates these property values from one state to another, creating continuous motion and visual transitions.
![animation](vg/ide/gifs/animation.gif)
![animation2](vg/ide/gifs/animation2.gif)