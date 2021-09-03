# Wolrd map data visualization

### Description: 
The project I made is a basic web application, using flask, that extracs information from a webpage, in this case from [knoema](https://knoema.com/ATLAS),
using web scrapping. This web application will show the user the CO2 emissions and population per country since 1970 in a colored map depending
on the value of the two variables. If the names in the source page where I extract the information don´t change, my project will automatically
extract the new information, since I made that this in a way it extracts all the data avaiable.

Such as any web application my project as a static folder, a templates folder, a text file with the requirements and the python script.

#### Python script:

###### App:
This is the program that will makw this web application work, you can see the comments to know what each line makes and some of the variable.
I recommend you see it first to understand better the templates files.

#### Templates folder:
This folder contains all the templates used on this project and future ones.

###### Index:
This template is mainly constituted by the svg file that I downlaoded, thtat I didn't change for thsi template and an unordered list that acts as 
the navigation bar.

###### CO2 & Population:
This two templates are the same only the variables change name, it as navigation bar like the other and the svg of the world map, but in this templates
the svg file was altered to show all the information and the correct colors, inside each path there is condition to make sure that, for that country
there is information avaiable if there is it will print the color associate to it if there isn't it will print grey.
In this templates is also define a tooltip create with the title tag, it was creade condition to make sure it exists if so, show tooltip. At first I didn't
want to use this method to create a popup with the information, I wanted something pretier and personalized but after many tries this was the best I could do.
The information added to the svg is very repetitive it only changes the name of the country, so I used a python script with regex to read the html file
and write the new information, based on the ID that came with svg file and another file that had ID associated with name of the country.
There is also another option is the navigation bar, it is an input with options that let's the user choose the year they want to see, at first I wanted to
do a scroll bar type of thing, but I end up doing this that looks more clean and was easier to implement.


#### Static folder:
This folder contains two css files, index_style and style.

###### Index_style:
This file defines the style proporties to the index template, it will define the background color, the colors and thickness of the borders of the svg 
map, and the navigation bar.

###### Style:
This file defines the style proporties of the templates with the information to be shown, it will define the same things as the index_style but it will
also change the properties of the svg map on hover and define the position and dimensions of the legend.


