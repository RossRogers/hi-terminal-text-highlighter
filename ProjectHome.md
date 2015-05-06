With this tool you can change terminal text like this:

&lt;BR&gt;


<table><tr><td><a href='http://picasaweb.google.com/lh/photo/6X0lhOwpVuLKpSkQ3X99og?feat=embedwebsite'><img src='http://lh3.ggpht.com/_YCibTrcxavs/S0Sb_1F3D2I/AAAAAAAAAlI/_kZCeiRmo74/s800/text_ex1_untransformed.png' /></a></td></tr><tr><td></td></tr></table>



&lt;BR&gt;


Into a color "`hi`"-lighted form like this:

&lt;BR&gt;


<table><tr><td><a href='http://picasaweb.google.com/lh/photo/kEbg4B6MaNwGHBsEMOLk6g?feat=embedwebsite'><img src='http://lh6.ggpht.com/_YCibTrcxavs/S0Scqf-YsFI/AAAAAAAAAlo/YLARWTsv6nA/s800/text_ex1_transformed.png' /></a></td></tr><tr><td></td></tr></table>

by filtering a log file with a command like this:
```
cat logfile | hi --hex_nums \
    ERROR RED NONE \
    '^ *\d+ ns \S* *:' BLACK NONE \
    injected BROWN NONE \
    sunk GREEN NONE | \
less -R

```

This tool was originally created to highlight specific text in verilog hardware simulation   log files.  It makes debug and navigation quicker.

## Installation ##
### Installation Pre-requisites ###


&lt;UL&gt;




&lt;LI&gt;

gcc

&lt;/LI&gt;




&lt;LI&gt;

[libboost-regex](http://www.boost.org/)

&lt;/LI&gt;




&lt;LI&gt;

[libboost-program-options](http://www.boost.org/)

&lt;/LI&gt;




&lt;/UL&gt;



#### Getting Installation Pre-requisites in Ubuntu ####
To get the ubuntu prerequisites, run the following ( must have root permissions ):

&lt;BR&gt;


`sudo apt-get install libboost-regex-dev libboost-program-options-dev`

### Download the Source ###
**[here](http://code.google.com/p/hi-terminal-text-highlighter/downloads/list)**

### Run make and install ###


&lt;UL&gt;




&lt;LI&gt;

 `tar xvjf hi.tar.bz2` 

&lt;/LI&gt;




&lt;LI&gt;

 `cd hi` 

&lt;/LI&gt;




&lt;LI&gt;

 `make` 

&lt;/LI&gt;




&lt;LI&gt;

 `mkdir ~/bin` 

&lt;/LI&gt;




&lt;LI&gt;

 `cp hi ~/bin` 

&lt;/LI&gt;




&lt;LI&gt;

  `echo export PATH="$HOME/bin:$PATH" >> ~/.bashrc` 

&lt;/LI&gt;




&lt;/UL&gt;



## Usage ##

<table><tr><td><a href='https://picasaweb.google.com/lh/photo/SfHAiyOg9ZNmWczsfLBaU9MTjNZETYmyPJy0liipFm0?feat=embedwebsite'><img src='https://lh6.googleusercontent.com/-39B3hocMR9U/UCuoBaG5BpI/AAAAAAAABrQ/H9BlMo6MIy4/s800/hi.1.1.0.help_screen.png' height='647' width='800' /></a></td></tr><tr><td>From <a href='https://picasaweb.google.com/115552508915031966747/Stuff?authuser=0&feat=embedwebsite'>stuff</a></td></tr></table>