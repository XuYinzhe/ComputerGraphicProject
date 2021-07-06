# Inpressionist
## Video Processing
After selecting the video, you need to keep moving your mouse on the `paintView` to let the process continue. I don't know why but it seems to related to OpenGL and FLTK multithreading?

 (also some other function may need this kind of action to take effect)

Anyway, after your mouse-moving, you will get a video in root of the project called `result.avi` (one is already generated, so user input may be needed when output)

Also, program may crush when input is large in resolution

## Blur brush
Blur Brush is named Gaussian Blur. It takes input from Original view, so it will be better to use the "swap" function when you need to blur something

## Kernal
The input format of kernal is like this:

For a matrix
>1 , 2 , 3
>
>4 , 5 , 6
>
>7 , 8 , 9

Input should be

`1,2,3,4,5,6,7,8,9`

IMPOTRANT
--
Must run this project in debug

Must run .exe in root or in VS